#ifndef APP_H
#define APP_H

#include "stm32h7xx_hal.h"
#include "top.h"
#include "de.h"
#include "nfem.h"
#include "ntcm.h"

#ifndef HSEM_ID_1
#define HSEM_ID_1 (1U)   /* HSEM_ID_0 is used for boot sync — use ID_1 for IDPS */
#endif

#define WINDOW_DURATION_MS  120000ULL
#define IDPS_SRAM4_ADDR    ((volatile uint32_t *)0x38000000U)  /* SRAM4 D3 */

void idps_app_init(void);
void idps_app_run(void);

#endif /* APP_H */
