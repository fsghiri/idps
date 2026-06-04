#ifndef NTCM_H
#define NTCM_H

#include "idps.h"

ParseResult_t ntcm(const uint8_t *buf, int len, Packet_t *out);

#endif /* NTCM_H */
