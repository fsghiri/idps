/*
 * ntcm_drain() → nfem_update() → nfem_finalize() → dt_predict() → SRAM4 → HSEM → CM4
 * */

#include "../inc/app.h"

static uint32_t win_start_ms = 0;

void idps_app_init(void)
{
    nfem_init();
    win_start_ms = HAL_GetTick();
}

void idps_app_run(void)
{
    // drain ring — filter + accumulate
    Packet_t pkt;
    while (ntcm_drain(&pkt))
    {
        nfem_update(&pkt);
    }

    // check window boundary
    if ((uint32_t)(HAL_GetTick() - win_start_ms) >= WINDOW_DURATION_MS)
    {
        float feat[IEC104_FEATURE_COUNT];
        nfem_finalize(feat);

        // DE: CART inference on CM7
        IEC104_Label label = (IEC104_Label)dt_predict(feat);


        if(label != LABEL_NORMAL){
        	// write to SRAM4
        	*IDPS_SRAM4_ADDR = (uint32_t)label;

        	// HSEM release
        	HAL_HSEM_FastTake(HSEM_ID_1);
        	HAL_HSEM_Release(HSEM_ID_1, 0);
        }

        win_start_ms = HAL_GetTick();
    }
}
