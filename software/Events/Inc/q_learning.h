#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <stdint.h>

#define Q_NUM_STATES   8
#define Q_NUM_ACTIONS  4

typedef enum
{
    ACTION_LOW_POWER = 0,
    ACTION_QRS_ANALYSIS,
    ACTION_DETAILED_ANALYSIS,
    ACTION_ALERT
} Q_Action_t;

void QLearning_Init(void);

Q_Action_t QLearning_SelectAction(
    uint8_t state
);

void QLearning_Update(
    uint8_t state,
    uint8_t action,
    float reward,
    uint8_t next_state
);

float QLearning_GetValue(
    uint8_t state,
    uint8_t action
);

#endif
