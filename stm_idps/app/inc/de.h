#ifndef DE_H
#define DE_H

#include "idps.h"

static inline int dt_predict(const float *x) {
    if (x[13] <= 9.5f) {
        if (x[9] <= 10.267857074737549f) {
            if (x[14] <= 2.5f) {
                if (x[4] <= 0.003140554763376713f) {
                    if (x[12] <= 42.93333435058594f) {
                        if (x[3] <= 0.7142049968242645f) {
                            if (x[8] <= 0.000484466552734375f) {
                                if (x[10] <= 5.367330074310303f) {
                                    return LABEL_DOS;
                                }
                                else {
                                    return LABEL_INJECTION;
                                }
                            }
                            else {
                                return LABEL_DOS;
                            }
                        }
                        else {
                            return LABEL_DOS;
                        }
                    }
                    else {
                        return LABEL_INJECTION;
                    }
                }
                else {
                    if (x[11] <= 25.789772987365723f) {
                        if (x[8] <= 0.002480030059814453f) {
                            return LABEL_DOS;
                        }
                        else {
                            return LABEL_INJECTION;
                        }
                    }
                    else {
                        return LABEL_INJECTION;
                    }
                }
            }
            else {
                return LABEL_INJECTION;
            }
        }
        else {
            return LABEL_DOS;
        }
    }
    else {
        return LABEL_NORMAL;
    }
}

#endif /* DE_H */
