#include "main.h"

// set memory regions of each module
Packet_t ntcm_ring[RING_SLOTS] __attribute__((section(".NTCMSection")));
Flow flow __attribute__((section(".NFEMSection")));
float feat[IEC104_FEATURE_COUNT] __attribute__((section(".DESection")));

ETH_DMADesc_t DMARxDscrTab[RING_SLOTS];
void ETH_Init(void){
    // RMII
    SYSCFG->PMCR |= SYSCFG_PMCR_EPIS_SEL_2;

    // ETH hardware init (clocks + GPIO)
    ETH_HW_Init();

    // configure MAC address
    static const uint8_t MACAddr[6] = {0x00, 0x80, 0xE1, 0x00, 0x00, 0x00};
    ETH->MACA0HR = ((uint32_t)MACAddr[5] << 8)  |  (uint32_t)MACAddr[4];
    ETH->MACA0LR = ((uint32_t)MACAddr[3] << 24) | ((uint32_t)MACAddr[2] << 16)
                 | ((uint32_t)MACAddr[1] << 8)  |  (uint32_t)MACAddr[0];

    // promiscuous mode, disable DA inverse filter
    ETH->MACPFR |=  ETH_MACPFR_PM;
    ETH->MACPFR &= ~ETH_MACPFR_DAIF;

    // enable timestamping PTP
    uint32_t ssinc_value = 16;
    ETH->MACTSCR |= ETH_MACTSCR_TSENA;
    ETH->MACSSIR = ssinc_value;
    ETH->MACSTSUR = 0;
    ETH->MACSTNUR = 0;
    ETH->MACTSCR |= ETH_MACTSCR_TSINIT;

    // Optional: wait for hardware to clear TSINIT
    while (ETH->MACTSCR & ETH_MACTSCR_TSINIT)
    {
    }

    // init DMA descriptor table — point each descriptor at its SRAM1 slot
    for (uint32_t i = 0; i < RING_SLOTS; i++) {
    	DMARxDscrTab[i].RDES0 = 0;
    	DMARxDscrTab[i].RDES1 = 0;
    	DMARxDscrTab[i].RDES2 = (uint32_t)ntcm_ring[i].data;
    	DMARxDscrTab[i].RDES3 = 0x80000000U     // OWN bit
    							| 0x40000000U   // IOC (interrupt on completion)
								| 0x01000000U;  // BUF1V (Buffer 1 Address Valid)
    }

    // point DMA to descriptor ring
    ETH->DMACRDLAR = (uint32_t)&DMARxDscrTab[0];
    ETH->DMACRDRLR = RING_SLOTS - 1;
    ETH->DMACRDTPR = (uint32_t)&DMARxDscrTab[RING_SLOTS - 1];

    // enable Rx DMA channel and MAC receiver
    ETH->DMACRCR |= ETH_DMACRCR_SR;
    ETH->MACCR   |= ETH_MACCR_RE;

    // enable Rx interrupts
    ETH->DMACIER |= ETH_DMACIER_RIE | ETH_DMACIER_RBUE;
}

volatile uint8_t eth_rx_ready = 0;

int main(void){
    // system clock and CMSIS SysTick
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 10); //fire an interrupt every 0.1s

    // dual core handshake: mode 2 (simultaneous boot)
    HSEM_Init();
    HSEM_Release();

    // peripheral init
    ETH_Init();

    // main app
    nfem_init(&flow); // init nfem module accumulator
    uint32_t win_start = GetTick();
    uint32_t win_actual = 0;

    while(1){
        if (eth_rx_ready){
            eth_rx_ready = 0;  // clear flag
            
            // process all available slots
            for (uint32_t i = 0; i < RING_SLOTS; i++){
                // check OWN bit (bit 31): 0 = CPU owns (data ready)
                if ((DMARxDscrTab[i].RDES3 & 0x80000000U) == 0){
                    
                    // descriptor table (packet length)
                    uint32_t frame_len = (DMARxDscrTab[i].RDES3 >> 0) & 0x3FFF;
                    ntcm_ring[i].len = frame_len;

                    // descriptor table PTP (timestamps)
                    uint32_t ts_sec = DMARxDscrTab[i].RDES0;
                    uint32_t ts_ns = DMARxDscrTab[i].RDES1;
                    ntcm_ring[i].timestamp = (ts_sec * 1000000ULL) + (ts_ns / 1000);
                    
                    // ntcm() => PARSE_OK
                    if(ntcm(ntcm_ring[i].data, ntcm_ring[i].len, &ntcm_ring[i]) == PARSE_OK){
                        // update accumulator
                        win_actual = GetTick();
                        nfem_update(&ntcm_ring[i], &flow);

                        // if window elapsed
                        if((win_actual - win_start)*10 >= WINDOW_DURATION){
                            // start new window
                            win_start = GetTick();

                            // push features
                            nfem_finalize(&flow, feat);

                            // run DE
                            if(dt_predict(feat) != LABEL_NORMAL){
                                // HSEM interrupt to CM4
                            }
                        }
                    }
                    
                    // drop slot => return ownership to DMA
                    DMARxDscrTab[i].RDES3 = 0x80000000U | 0x40000000U | 0x01000000U;
                }
            }
        }
    }
}

