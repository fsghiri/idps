
#define TS_N_STRATEGIES   2
#define TS_S_ISOLATE      0    /* Block / alert + block on STM32 */
#define TS_S_WAIT         1    /* Log only, notify on threshold  */

/* Pre-trained Beta posteriors — paste into thompson_init() */
static const float TS_ALPHA_INIT[TS_N_STRATEGIES] = {
    878.0f,   /* s0: ISOLATE  — alpha */
    1.0f    /* s1: WAIT     — alpha */
};

static const float TS_BETA_INIT[TS_N_STRATEGIES] = {
    81.0f,    /* s0: ISOLATE  — beta  */
    4.0f     /* s1: WAIT     — beta  */
};

/*
 * Posterior means at deployment:
 *   theta_0 (isolate) = 0.9155
 *   theta_1 (wait)    = 0.2000
 *   Dominant strategy : ISOLATE
 *
 * Cost table used during training:
 *   dos       -> ISOLATE (cost isolate=1, cost wait=9)
 *   injection -> ISOLATE (cost isolate=2, cost wait=8)
 *   normal    -> WAIT    (cost isolate=8, cost wait=0)
 */
