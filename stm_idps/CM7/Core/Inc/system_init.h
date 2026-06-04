#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <stdint.h>

/* ============================================================================
 * SYSTEM CLOCK & BOOT CONFIGURATION
 * ============================================================================ */
void SystemClock_Config(void);
void HSEM_Init(void);
void HSEM_Release(void);
void GPIO_Init(void);
void ETH_HW_Init(void);

#endif /* SYSTEM_INIT_H */
