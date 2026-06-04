#include "main.h"

/*==================== CM7 Exception Handlers ====================*/
void NMI_Handler(void) { while (1); }
void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void) { while (1); }
void UsageFault_Handler(void) { while (1); }

//systick
volatile uint32_t systick_ms = 0;
void SysTick_Handler(void){
    systick_ms++;
}

// GetTick
uint32_t GetTick(void) {
    return systick_ms;
}

// error
void Error_Handler(void){
  __disable_irq();
}

/*==================== IRQ HANDLERs ====================*/
// HSEM
void HSEM1_IRQHandler(void){
    // clear HSEM interrupt
    uint32_t status = HSEM->C1MISR;
    HSEM->C1ICR = status;
}

// ethernet: fires when finished writing a packet into ring
extern volatile uint8_t eth_rx_ready;
void ETH_IRQHandler(void) {
    // flag a packet is ready
    eth_rx_ready = 1;
    
    // clear interrupt flags
    ETH->DMACSR |= ETH_DMACSR_RI | ETH_DMACSR_RBU;
}
