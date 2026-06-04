#include "../inc/nfem.h"

/* Welford online statistics */
static void stat_init(Stat *s){
    s->n    = 0;
    s->mean = 0.0;
    s->M2   = 0.0;
    s->min  = DBL_MAX;
    s->max  = -DBL_MAX;
}
static void stat_push(Stat *s, double x){
    s->n++;
    double delta = x - s->mean;
    s->mean += delta / (double)s->n;
    s->M2   += delta * (x - s->mean);
    if (x < s->min) s->min = x;
    if (x > s->max) s->max = x;
}

static double stat_var(const Stat *s) { return s->n < 2 ? 0.0 : s->M2 / (double)s->n; }
static double stat_std(const Stat *s) { return sqrt(stat_var(s)); }
static double stat_min(const Stat *s) { return s->min ==  DBL_MAX ? 0.0 : s->min; }
static double stat_max(const Stat *s) { return s->max == -DBL_MAX ? 0.0 : s->max; }

/* APDU scanner */
static ApduScan scan_apdus(const uint8_t *tcp_payload, int payload_len){
    ApduScan result = {0};
    int i = 0;

    while (i < payload_len - 1 && result.count < MAX_APDU_PER_PKT) {
        if (tcp_payload[i] != IEC104_START) { i++; continue; }

        int apci_len  = (int)tcp_payload[i + 1];
        int total_len = 2 + apci_len;

        if (apci_len < APCI_MIN_LEN)  { i++; continue; }
        if (i + total_len > payload_len) break;

        result.lens[result.count++] = (uint32_t)total_len;

        uint8_t ctrl = tcp_payload[i + 2];
        if ((ctrl & 0x01u) && (ctrl & 0x02u))  /* U-format */
            result.u_count++;

        i += total_len;
    }
    return result;
}

static TcpPayload get_tcp_payload(const uint8_t *frame, int frame_len){
    TcpPayload result = {0};

    if (frame_len < ETH_HDR_LEN + 20) return result;

    const uint8_t *ip      = frame + ETH_HDR_LEN;
    int            ihl     = (ip[0] & 0x0Fu) * 4;
    const uint8_t *tcp     = ip + ihl;
    int            tcp_off = ((tcp[12] >> 4) & 0x0Fu) * 4;
    const uint8_t *payload = tcp + tcp_off;

    result.data = payload;
    result.len  = frame_len - (int)(payload - frame);
    return result;
}

/* Burst / idle tracker update */
static void update_bursts(Flow *flow, uint64_t ts){
    if (!flow->seen) {
        flow->burst_start = ts;
        flow->burst_last  = ts;
        flow->last_ts     = ts;
        flow->seen        = 1;
        return;
    }

    uint64_t gap = ts - flow->last_ts;

    if (gap >= IDLE_THRESHOLD_US) {
        double active_dur = (double)(flow->burst_last - flow->burst_start);
        if (active_dur >= 0.0) stat_push(&flow->active_time, active_dur);
        stat_push(&flow->idle_time, (double)gap);
        flow->burst_start = ts;
    }

    flow->burst_last = ts;
    flow->last_ts    = ts;
}

// init a flow in sram2 to 0
void nfem_init(Flow *flow){
    memset(flow, 0, sizeof(Flow));
    stat_init(&flow->apdu_all);
    stat_init(&flow->apdu_bw);
    stat_init(&flow->iat_bw);
    stat_init(&flow->active_time);
    stat_init(&flow->idle_time);
}

// update flow
void nfem_update(Packet_t *pkt, Flow *flow){	
    uint64_t   ts      = pkt->timestamp;
    IEC104Dir  dir     = pkt->direction;
    TcpPayload payload = get_tcp_payload(pkt->data, pkt->len);

    if (!payload.data || payload.len <= 0) return;

    ApduScan scan = scan_apdus(payload.data, payload.len);
    if (scan.count == 0) return;

    // APDU length stats
    for (int a = 0; a < scan.count; a++) {
        stat_push(&flow->apdu_all, (double)scan.lens[a]);
        if (dir == IEC104_BW)
            stat_push(&flow->apdu_bw, (double)scan.lens[a]);
    }

    // U-message counters
    flow->u_total += (uint32_t)scan.u_count;
    if (dir == IEC104_BW) flow->u_bw += (uint32_t)scan.u_count;

    // BW inter-arrival time
    if (dir == IEC104_BW) {
        if (flow->last_bw_ts > 0)
            stat_push(&flow->iat_bw, (double)(ts - flow->last_bw_ts));
        flow->last_bw_ts = ts;
    }

    update_bursts(flow, ts);
}

// flow -> feat
void nfem_finalize(Flow *flow, float feat[IEC104_FEATURE_COUNT]){
    // close trailing active burst
    if (flow->seen) {
        double dur = (double)(flow->burst_last - flow->burst_start);
        if (dur >= 0.0) stat_push(&flow->active_time, dur);
    }

    if (flow->idle_time.n   == 0) stat_push(&flow->idle_time,   0.0);
    if (flow->active_time.n == 0) stat_push(&flow->active_time, 0.0);
    if (flow->iat_bw.n      == 0) stat_push(&flow->iat_bw,      0.0);

    // fill feature vector
    feat[0]  = (float)(stat_max(&flow->idle_time));
    feat[1]  = (float)(stat_min(&flow->idle_time));
    feat[2]  = (float)(flow->idle_time.mean);
    feat[3]  = (float)(stat_var(&flow->idle_time));
    feat[4]  = (float)(stat_min(&flow->active_time));
    feat[5]  = (float)(flow->active_time.mean);
    feat[6]  = (float)(stat_std(&flow->active_time));
    feat[7]  = (float)(stat_var(&flow->active_time));
    feat[8]  = (float)(stat_min(&flow->iat_bw));
    feat[9]  = (float)flow->apdu_all.mean;
    feat[10] = (float)stat_std(&flow->apdu_all);
    feat[11] = (float)stat_var(&flow->apdu_all);
    feat[12] = (float)stat_var(&flow->apdu_bw);
    feat[13] = (float)flow->u_total;
    feat[14] = (float)flow->u_bw;

    // reset accumulator for next window
    nfem_init(flow);
}
