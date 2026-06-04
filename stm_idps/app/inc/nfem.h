#ifndef NFEM_H
#define NFEM_H

#include "idps.h"

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

void nfem_init(Flow *flow);
void nfem_update(Packet_t *pkt, Flow *flow);
void nfem_finalize(Flow *flow, float feat[IEC104_FEATURE_COUNT]);

#endif /* NFEM_H */
