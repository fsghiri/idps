#ifndef NTCM_H
#define NTCM_H

#include "top.h"

extern NTCM_RingBuffer_t ntcm_ring;

bool ntcm_drain(Packet_t *out);

#endif /* NTCM_H */
