#ifndef DE_H
#define DE_H

#include "top.h"

static inline int dt_predict(const float *x) {
    if (x[7] <= 9.5f) {
        if (x[3] <= 10.267857074737549f) {
            if (x[8] <= 2.5f) {
                if (x[1] <= 0.003140554763376713f) {
                    if (x[6] <= 42.93333435058594f) {
                        if (x[0] <= 0.7142049968242645f) {
                            if (x[2] <= 0.000484466552734375f) {
                                if (x[5] <= 28.84415626525879f) {
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
                    if (x[4] <= 5.077941417694092f) {
                        if (x[2] <= 0.002480030059814453f) {
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
