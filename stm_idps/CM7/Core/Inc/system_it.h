#ifndef SYSTEM_IT_H
#define SYSTEM_IT_H

#include <stdint.h>

/* ============================================================================
 * EXCEPTION HANDLERS
 * ============================================================================ */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void Error_Handler(void);
void SysTick_Handler(void);

/* ============================================================================
 * PERIPHERAL IRQ HANDLERS
 * ============================================================================ */
void ETH_IRQHandler(void);
void HSEM1_IRQHandler(void);

/* ============================================================================
 * GetTick
 * ============================================================================ */
uint32_t GetTick(void);
extern volatile uint32_t systick_ms;

#endif /* SYSTEM_IT_H */
