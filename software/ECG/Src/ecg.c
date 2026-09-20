#include "ecg.h"
#include "filter.h"
#include "qrs_detector.h"

static float filtered_sample = 0.0f;
static float heart_rate = 0.0f;
static float rr_interval = 0.0f;

static uint8_t beat_detected = 0;
static uint8_t signal_good = 0;

void ECG_Init(void)
{
    Filter_Init();
    QRS_Init();

    filtered_sample = 0.0f;
    heart_rate = 0.0f;
    rr_interval = 0.0f;

    beat_detected = 0;
    signal_good = 0;
}

void ECG_ProcessSample(uint16_t adc_value)
{
    filtered_sample = Filter_Process((float)adc_value);

    signal_good = Filter_IsSignalGood();

    beat_detected = QRS_Process(
        filtered_sample,
        &rr_interval,
        &heart_rate
    );
}

float ECG_GetFilteredSample(void)
{
    return filtered_sample;
}

float ECG_GetHeartRate(void)
{
    return heart_rate;
}

float ECG_GetRRInterval(void)
{
    return rr_interval;
}

uint8_t ECG_IsBeatDetected(void)
{
    return beat_detected;
}

uint8_t ECG_IsSignalGood(void)
{
    return signal_good;
}
