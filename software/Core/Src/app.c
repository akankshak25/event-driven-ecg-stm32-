#include "app.h"
#include "main.h"

#include "ecg.h"
#include "communication.h"
#include "events.h"
#include "q_learning.h"

extern ADC_HandleTypeDef hadc1;

static uint8_t previous_state = 0;
static uint8_t previous_action = 0;

void APP_Init(void)
{
    ECG_Init();
    Communication_Init();
    Events_Init();
    QLearning_Init();
}

void APP_ProcessSample(void)
{
    uint16_t adc_value;

    float heart_rate;
    float rr_interval;

    uint8_t beat_detected;
    uint8_t signal_good;

    ECG_Event_t event;
    ProcessingLevel_t level;

    /*
     * Read ADC.
     */
    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(
        &hadc1,
        10
    );

    adc_value =
        HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    /*
     * ECG processing.
     */
    ECG_ProcessSample(adc_value);

    heart_rate =
        ECG_GetHeartRate();

    rr_interval =
        ECG_GetRRInterval();

    beat_detected =
        ECG_IsBeatDetected();

    signal_good =
        ECG_IsSignalGood();

    /*
     * Send raw ECG to PC.
     */
    Communication_SendECG(adc_value);

    /*
     * Event-driven analysis.
     */
    event = Events_Update(
        heart_rate,
        rr_interval,
        signal_good,
        beat_detected
    );

    level =
        Events_GetProcessingLevel();

    /*
     * Convert current conditions
     * into the 8-state Q-learning space.
     *
     * bit 2 = signal quality
     * bit 1 = event
     * bit 0 = rhythm activity
     */
    uint8_t signal_state =
        signal_good ? 0 : 1;

    uint8_t event_state =
        (event != EVENT_NORMAL) ? 1 : 0;

    uint8_t rhythm_state =
        (heart_rate > 120.0f ||
         heart_rate < 45.0f) ? 1 : 0;

    uint8_t state =
        (signal_state << 2) |
        (event_state << 1) |
        rhythm_state;

    /*
     * Q-learning selects processing level.
     */
    Q_Action_t action =
        QLearning_SelectAction(state);

    /*
     * Simple reward model.
     */
    float reward = 0.0f;

    if (event == EVENT_NORMAL)
    {
        if (action == ACTION_LOW_POWER)
            reward = 5.0f;
        else
            reward = -1.0f;
    }
    else if (event == EVENT_QRS_DETECTED)
    {
        if (action == ACTION_QRS_ANALYSIS)
            reward = 5.0f;
        else
            reward = 1.0f;
    }
    else if (event == EVENT_RHYTHM_WARNING)
    {
        if (action == ACTION_DETAILED_ANALYSIS)
            reward = 7.0f;
        else
            reward = -3.0f;
    }

    /*
     * Update Q-table.
     */
    QLearning_Update(
        previous_state,
        previous_action,
        reward,
        state
    );

    previous_state = state;
    previous_action = action;

    /*
     * Send HR only when a new beat is detected.
     * This keeps UART traffic lower.
     */
    if (beat_detected)
    {
        Communication_SendHeartRate(
            heart_rate
        );

        Communication_SendRR(
            rr_interval
        );
    }
}
