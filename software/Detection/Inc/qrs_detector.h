#ifndef QRS_DETECTOR_H
#define QRS_DETECTOR_H

#include <stdint.h>

void QRS_Init(void);

uint8_t QRS_Process(
    float sample,
    float *rr_interval,
    float *heart_rate
);

#endif
