#include "main.h"

/*==================== SYSTEM CLOCK CONFIG ====================*/
void SystemClock_Config(void){
    // enable PWR clock
    RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;

    // enable SMPS mode
    PWR->CR3 &= ~PWR_CR3_LDOEN;
    PWR->CR3 |=  PWR_CR3_SMPSEN;

    // VOS 3
    PWR->D3CR &= ~PWR_D3CR_VOS;
    PWR->D3CR |=  PWR_D3CR_VOS_1;
    while ((PWR->D3CR & PWR_D3CR_VOSRDY) == 0U) {}


    // Enable HSI */
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {}

    // HSI divider = /1 (default) */
    RCC->CR &= ~RCC_CR_HSIDIV;

    // disable PLL
    RCC->CR &= ~RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) != 0U) {}

    // set SYSCLK to HSI
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |=  RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {}

    // prescalers (DIV1)
    RCC->D1CFGR &= ~(RCC_D1CFGR_HPRE | RCC_D1CFGR_D1PPRE);
    RCC->D1CFGR |=  (0U); 

    RCC->D2CFGR &= ~(RCC_D2CFGR_D2PPRE1 | RCC_D2CFGR_D2PPRE2);
    RCC->D2CFGR |=  (0U);

    RCC->D3CFGR &= ~(RCC_D3CFGR_D3PPRE);
    RCC->D3CFGR |=  (0U); 

    // flash latency
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |=  FLASH_ACR_LATENCY_1WS;
}

/*
 * ==================== HSEM ====================
 *
 * DUAL CORE BOOT SYNC SEQUENCE MODES: (mode 2)
 * 	mode 1: CM7 master & CM4 slave (held in stop mode)
 * 		1. CM7 initialize the system (clk, registers, ...) 
 * 		2. CM7 HSEM handshake to wake up CM4
 * 	mode 2: simultaneous boot
 * 		CM7 waits (timeout = 0xFFFF) for CM4 hold stop mode before starting init of system
 * 			while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0)
 *  		if timeout ends and still no flag => Error_Handler() == __disable_irq()
 * 
 */
// HSEM init
void HSEM_Init(void){
    // enable HSEM clock
    RCC->AHB4ENR |= RCC_AHB4ENR_HSEMEN;
    volatile uint32_t tmp = RCC->AHB4ENR; // small sync delay
    (void)tmp;

    NVIC_SetPriority(HSEM1_IRQn, 6); 
    NVIC_EnableIRQ(HSEM1_IRQn); 
}

// release
void HSEM_Release(void){
    // wait for CM4 to enter sleep mode
    uint32_t timeout = 0xFFFF;
    while ((RCC->CR & RCC_CR_D2CKRDY) != 0U && (timeout-- > 0)){}
    if (timeout <= 0)
        Error_Handler();

    // HSEM release
    HSEM->R[0] = (1U << 31);
    HSEM->R[0] = 0U;

    // wait for CM4 wakeup
    timeout = 0xFFFF;
    while ((RCC->CR & RCC_CR_D2CKRDY) == 0U && (timeout-- > 0)){}
    if (timeout <= 0)
        Error_Handler();
}

/*==================== peripheral hardware init ====================*/
// GPIO clock
void GPIO_Init(void){
    // enable GPIOA GPIOB GPIOC clocks
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
}

// ethernet clock and pins
void ETH_HW_Init(void){
    // enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_ETH1MACEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_ETH1TXEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_ETH1RXEN;

    // enable GPIO clocks
    GPIO_Init();

    /* ------------------------------------------------- */
    /* PC1  -> ETH_MDC                                   */
    /* PC4  -> ETH_RXD0                                  */
    /* PC5  -> ETH_RXD1                                  */
    /* ------------------------------------------------- */
    GPIO_AF_SET(GPIOC, 1, 11);
    GPIO_AF_SET(GPIOC, 4, 11);
    GPIO_AF_SET(GPIOC, 5, 11);

    /* ------------------------------------------------- */
    /* PA1 -> ETH_REF_CLK                                */
    /* PA2 -> ETH_MDIO                                   */
    /* PA7 -> ETH_CRS_DV                                 */
    /* ------------------------------------------------- */
    GPIO_AF_SET(GPIOA, 1, 11);
    GPIO_AF_SET(GPIOA, 2, 11);
    GPIO_AF_SET(GPIOA, 7, 11);

    /* ------------------------------------------------- */
    /* PB11 -> ETH_TX_EN                                 */
    /* PB12 -> ETH_TXD0                                  */
    /* PB13 -> ETH_TXD1                                  */
    /* ------------------------------------------------- */
    GPIO_AF_SET(GPIOB,11, 11);
    GPIO_AF_SET(GPIOB,12, 11);
    GPIO_AF_SET(GPIOB,13, 11);

    // enable ETH_IRQn interrupt line in the NVIC.
    NVIC_SetPriority(ETH_IRQn, 5);
    NVIC_EnableIRQ(ETH_IRQn);
}
