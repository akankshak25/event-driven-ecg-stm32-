#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>

void Communication_Init(void);

void Communication_SendECG(
    uint16_t adc_value
);

void Communication_SendHeartRate(
    float heart_rate
);

void Communication_SendRR(
    float rr_interval
);

#endif
