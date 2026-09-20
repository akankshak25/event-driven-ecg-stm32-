#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

typedef enum
{
    EVENT_NORMAL = 0,
    EVENT_QRS_DETECTED,
    EVENT_RHYTHM_WARNING,
    EVENT_SIGNIFICANT
} ECG_Event_t;

typedef enum
{
    PROCESS_LOW_POWER = 0,
    PROCESS_QRS_ANALYSIS,
    PROCESS_DETAILED_ANALYSIS,
    PROCESS_ALERT
} ProcessingLevel_t;

void Events_Init(void);

ECG_Event_t Events_Update(
    float heart_rate,
    float rr_interval,
    uint8_t signal_good,
    uint8_t beat_detected
);

ProcessingLevel_t Events_GetProcessingLevel(void);

#endif
