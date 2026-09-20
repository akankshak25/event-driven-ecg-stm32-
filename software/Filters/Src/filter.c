#include "filter.h"
#include <math.h>

/*
 * ECG preprocessing
 *
 * Stage 1:
 * High-pass style DC/baseline removal
 *
 * Stage 2:
 * Low-pass smoothing
 *
 * The resulting signal is suitable for
 * QRS/R-peak detection.
 */

static float dc_estimate = 0.0f;
static float lowpass_state = 0.0f;

static float signal_mean = 0.0f;
static float signal_variance = 0.0f;

#define DC_ALPHA        0.995f
#define LOWPASS_ALPHA   0.20f

void Filter_Init(void)
{
    dc_estimate = 0.0f;
    lowpass_state = 0.0f;

    signal_mean = 0.0f;
    signal_variance = 0.0f;
}

float Filter_Process(float input)
{
    float highpass;
    float output;

    /*
     * Baseline/DC removal
     */
    dc_estimate =
        DC_ALPHA * dc_estimate +
        (1.0f - DC_ALPHA) * input;

    highpass = input - dc_estimate;

    /*
     * Low-pass smoothing
     */
    lowpass_state =
        LOWPASS_ALPHA * highpass +
        (1.0f - LOWPASS_ALPHA) * lowpass_state;

    output = lowpass_state;

    /*
     * Running signal statistics
     */
    signal_mean =
        0.995f * signal_mean +
        0.005f * output;

    signal_variance =
        0.995f * signal_variance +
        0.005f *
        (output - signal_mean) *
        (output - signal_mean);

    return output;
}

uint8_t Filter_IsSignalGood(void)
{
    /*
     * Very small variance means that
     * there is probably no useful ECG.
     */
    if (signal_variance < 0.01f)
        return 0;

    return 1;
}
