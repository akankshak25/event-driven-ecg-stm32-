#include "events.h"

static ProcessingLevel_t processing_level;

void Events_Init(void)
{
    processing_level = PROCESS_LOW_POWER;
}

ECG_Event_t Events_Update(
    float heart_rate,
    float rr_interval,
    uint8_t signal_good,
    uint8_t beat_detected
)
{
    /*
     * LEVEL 1
     *
     * Poor signal:
     * continue basic monitoring.
     */
    if (!signal_good)
    {
        processing_level =
            PROCESS_LOW_POWER;

        return EVENT_NORMAL;
    }

    /*
     * LEVEL 2
     *
     * Possible QRS detected.
     */
    if (beat_detected)
    {
        processing_level =
            PROCESS_QRS_ANALYSIS;

        /*
         * Heart-rate range is only being
         * used as an event trigger here.
         */
        if (heart_rate > 120.0f ||
            heart_rate < 45.0f)
        {
            processing_level =
                PROCESS_DETAILED_ANALYSIS;

            return EVENT_RHYTHM_WARNING;
        }

        return EVENT_QRS_DETECTED;
    }

    /*
     * LEVEL 1
     */
    processing_level =
        PROCESS_LOW_POWER;

    return EVENT_NORMAL;
}

ProcessingLevel_t Events_GetProcessingLevel(void)
{
    return processing_level;
}
