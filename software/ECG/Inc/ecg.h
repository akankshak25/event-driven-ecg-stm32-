#ifndef ECG_H
#define ECG_H

#include <stdint.h>

#define ECG_SAMPLE_RATE_HZ    250U

void ECG_Init(void);
void ECG_ProcessSample(uint16_t adc_value);

float ECG_GetFilteredSample(void);
float ECG_GetHeartRate(void);
float ECG_GetRRInterval(void);

uint8_t ECG_IsBeatDetected(void);
uint8_t ECG_IsSignalGood(void);

#endif
