#include "qrs_detector.h"
#include <math.h>

#define FS                  250.0f

#define REFRACTORY_SAMPLES  75U
#define MIN_RR_SECONDS      0.40f
#define MAX_RR_SECONDS      2.00f

#define THRESHOLD_DECAY     0.995f
#define THRESHOLD_FACTOR    3.0f

static float previous_sample = 0.0f;
static float previous_derivative = 0.0f;

static float energy = 0.0f;
static float threshold = 0.0f;

static uint32_t sample_counter = 0;
static uint32_t last_peak = 0;

static uint8_t initialized = 0;

void QRS_Init(void)
{
    previous_sample = 0.0f;
    previous_derivative = 0.0f;

    energy = 0.0f;
    threshold = 0.0f;

    sample_counter = 0;
    last_peak = 0;

    initialized = 0;
}

uint8_t QRS_Process(
    float sample,
    float *rr_interval,
    float *heart_rate
)
{
    float derivative;
    float squared;

    uint32_t interval_samples;

    *rr_interval = 0.0f;

    if (!initialized)
    {
        previous_sample = sample;
        initialized = 1;

        sample_counter++;
        return 0;
    }

    /*
     * Derivative emphasizes QRS complexes.
     */
    derivative = sample - previous_sample;

    previous_sample = sample;

    /*
     * Square to make energy positive.
     */
    squared = derivative * derivative;

    /*
     * Moving energy estimate.
     */
    energy =
        0.90f * energy +
        0.10f * squared;

    /*
     * Adaptive threshold.
     */
    threshold =
        THRESHOLD_DECAY * threshold +
        (1.0f - THRESHOLD_DECAY)
        * energy * THRESHOLD_FACTOR;

    sample_counter++;

    /*
     * Candidate QRS event.
     */
    if (energy > threshold)
    {
        /*
         * Refractory period prevents
         * detecting the same QRS multiple times.
         */
        if ((sample_counter - last_peak)
            >= REFRACTORY_SAMPLES)
        {
            if (last_peak != 0)
            {
                interval_samples =
                    sample_counter - last_peak;

                *rr_interval =
                    (float)interval_samples / FS;

                if ((*rr_interval >= MIN_RR_SECONDS) &&
                    (*rr_interval <= MAX_RR_SECONDS))
                {
                    *heart_rate =
                        60.0f / (*rr_interval);

                    last_peak = sample_counter;

                    return 1;
                }
            }
            else
            {
                last_peak = sample_counter;
            }
        }
    }

    return 0;
}
