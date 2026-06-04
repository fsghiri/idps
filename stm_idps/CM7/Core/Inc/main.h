#ifndef __MAIN_H
#define __MAIN_H

/*
 * ========================================================================================
 * INCLUDES
 * ========================================================================================
 */
#include "stm32h7xx.h"
#include "stm32h755xx.h"
#include "system_stm32h7xx.h"

#include "idps.h"
#include "system_it.h"
#include "system_init.h"

/*
 * ========================================================================================
 * DEFENITIONS
 * ========================================================================================
 */

#define RING_SLOTS 64U
#define ETH_RX_BUF_SIZE 1536U

// macro helper
#define GPIO_AF_SET(port, pin, af)                                            \
do {                                                                          \
    (port)->MODER    &= ~(3U << ((pin) * 2));                                 \
    (port)->MODER    |=  (2U << ((pin) * 2));                                 \
    (port)->OSPEEDR  &= ~(3U << ((pin) * 2));                                 \
    (port)->OSPEEDR  |=  (3U << ((pin) * 2));                                 \
    (port)->AFR[(pin) >> 3] &= ~(15U << (((pin) & 0x07) * 4));                \
    (port)->AFR[(pin) >> 3] |= ((uint32_t)(af) << (((pin) & 0x07) * 4));      \
} while(0)

/*
 * ========================================================================================
 * STRUCTS
 * ========================================================================================
 */

// DMA descriptor table in .bss
typedef struct {
    /* hardware DMA strictly expects a 4-word (16-byte) descriptor in memory */
	__IO uint32_t RDES0;
	__IO uint32_t RDES1;
	__IO uint32_t RDES2;   /* buffer pointer */
	__IO uint32_t RDES3;   /* OWN bit + frame length */
} ETH_DMADesc_t;

/*
 * ========================================================================================
 * EXTERNAL DECLARATIONS
 * ========================================================================================
 */

extern volatile uint32_t systick_ms;

#endif /* __MAIN_H */
