#ifndef NFEM_H
#define NFEM_H

#include "top.h"

/*
static const char *FEATURE_NAMES[IEC104_FEATURE_COUNT] = {
    "flow idle time max",
    "flow idle time min",
    "flow idle time mean",
    "flow idle time variance",
    "flow active time min",
    "flow active time mean",
    "flow active time std",
    "flow active time variance",
    "bw IAT min",
    "flow packet APDU length mean",
    "flow packet APDU length std",
    "flow packet APDU length var",
    "bw packet APDU length var",
    "flow total IEC104_U_Message packets",
    "bw total IEC104_U_Message packets",
};
*/

void nfem_init(void);
void nfem_update(const Packet_t *pkt);
void nfem_finalize(float feat[IEC104_FEATURE_COUNT]);

#endif /* NFEM_H */
