#ifndef TOP_H
#define TOP_H

/*
 * ========================================================================================
 * INCLUDES
 * ========================================================================================
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * ========================================================================================
 * DEFENITIONS
 * ========================================================================================
 */

/*NTCM*/
#define IEC104_PORT     2404
#define ETH_HDR_LEN     14
#define IPV4_ETHERTYPE  0x0800
#define IP_PROTO_TCP    6
#define NTCM_RX_RING_SIZE   16U

/*NFEM DEFINITIONS*/
#define ETH_HDR_LEN           14
#define IEC104_START          0x68u
#define APCI_MIN_LEN          4
#define MAX_APDU_PER_PKT      16
#define IDLE_THRESHOLD_US     1000000ULL
#define IEC104_FEATURE_COUNT  15
#define WINDOW_DURATION_US 120 * 1000000ULL

/*
 * ========================================================================================
 * STRUCTS
 * ========================================================================================
 */

/*NTCM STRUCTS*/
typedef struct {
    uint8_t  data[1536];
    uint16_t length;
    uint32_t timestamp_ms;
    uint8_t  valid;
} NTCM_FrameSlot_t;

typedef struct {
    NTCM_FrameSlot_t slots[NTCM_RX_RING_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} NTCM_RingBuffer_t;

typedef enum {
    PARSE_OK,
    PARSE_DROP_NOT_IPV4,
    PARSE_DROP_NOT_TCP,
    PARSE_DROP_FRAGMENTED,
    PARSE_DROP_NOT_IEC104,
} ParseResult_t;

/* Packet direction */
typedef enum { IEC104_FW = 0, IEC104_BW = 1 } IEC104Dir;

typedef struct {
    const uint8_t *data;    // ntcm
    int len;                // given now but should be calculated from ethernet stream
    uint32_t src_ip;        // ntcm
    uint32_t dst_ip;        // ntcm
    uint64_t timestamp;
    IEC104Dir direction;    // ntcm
} Packet_t;

/*NFEM*/
// Welford online statistics
typedef struct {
    uint64_t n;
    double   mean;
    double   M2;
    double   min;
    double   max;
} Stat;

typedef struct {
    uint32_t lens[MAX_APDU_PER_PKT];
    int      count;
    int      u_count;   // U-format APDUs (ctrl byte bits[1:0] == 11)
} ApduScan;

typedef struct {
    Stat     apdu_all;        // APDU length, all directions
    Stat     apdu_bw;         // APDU length, backward only
    Stat     iat_bw;          // inter-arrival time, backward only
    Stat     active_time;     // duration of each completed active burst
    Stat     idle_time;       // duration of each idle gap

    uint32_t u_total;         // U-format APDU count, all directions
    uint32_t u_bw;            // U-format APDU count, backward only

    uint64_t last_ts;         // timestamp of previous packet (any dir)
    uint64_t last_bw_ts;      // timestamp of previous BW packet
    uint64_t burst_start;     // start of current active burst
    uint64_t burst_last;      // last packet in current active burst
    int      seen;            // 0 until first packet is processed
} Flow;

/*DE*/
typedef enum {
    LABEL_NORMAL = 0,
    LABEL_DOS = 1,
    LABEL_INJECTION = 2
} IEC104_Label;

#endif /* TOP_H*/
