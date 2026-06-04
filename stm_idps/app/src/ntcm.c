#include "../inc/ntcm.h"

static uint16_t be16(const uint8_t *p) { 
    return (uint16_t)((p[0]<<8)|p[1]); 
}
static uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];
}

//takes in raw bytes -> if protocol == IEC 104 => get direction, src_ip, dst_ip
ParseResult_t ntcm(const uint8_t *buf, int len, Packet_t *out){
    if (be16(buf + 12) != IPV4_ETHERTYPE) 
        return PARSE_DROP_NOT_IPV4;

    const uint8_t *ip  = buf + ETH_HDR_LEN;
    uint8_t        ihl = (ip[0] & 0x0f) * 4;

    if (ip[9] != IP_PROTO_TCP)            
        return PARSE_DROP_NOT_TCP;
    if (be16(ip + 6) & 0x3fff)            
        return PARSE_DROP_FRAGMENTED;

    const uint8_t *tcp = ip + ihl;
    uint16_t sport     = be16(tcp + 0);
    uint16_t dport     = be16(tcp + 2);

    if (dport == IEC104_PORT){
        out->direction = IEC104_FW; // Forward: SCADA -> RTU (destination port == 2404)
    } else if (sport == IEC104_PORT) {
        out->direction = IEC104_BW; // Backward: RTU -> SCADA (source port == 2404)
    } else {
        return PARSE_DROP_NOT_IEC104;
    }

    uint8_t tcp_hdr_len = ((tcp[12] >> 4) & 0x0f) * 4;
    const uint8_t *pay = tcp + tcp_hdr_len;
    int pay_len= len - (int)(pay - buf);

    if (pay_len < 6 || pay[0] != 0x68)    
        return PARSE_DROP_NOT_IEC104;

    out->src_ip = be32(ip + 12);
    out->dst_ip = be32(ip + 16);
    return PARSE_OK;
}

