#include "q_learning.h"

#define ALPHA       0.10f
#define GAMMA       0.90f

static float Q[Q_NUM_STATES][Q_NUM_ACTIONS];

void QLearning_Init(void)
{
    for (uint8_t s = 0; s < Q_NUM_STATES; s++)
    {
        for (uint8_t a = 0; a < Q_NUM_ACTIONS; a++)
        {
            Q[s][a] = 0.0f;
        }
    }
}

Q_Action_t QLearning_SelectAction(uint8_t state)
{
    uint8_t best_action = 0;

    float best_value = Q[state][0];

    for (uint8_t a = 1; a < Q_NUM_ACTIONS; a++)
    {
        if (Q[state][a] > best_value)
        {
            best_value = Q[state][a];
            best_action = a;
        }
    }

    return (Q_Action_t)best_action;
}

void QLearning_Update(
    uint8_t state,
    uint8_t action,
    float reward,
    uint8_t next_state
)
{
    float max_next = Q[next_state][0];

    for (uint8_t a = 1; a < Q_NUM_ACTIONS; a++)
    {
        if (Q[next_state][a] > max_next)
        {
            max_next = Q[next_state][a];
        }
    }

    /*
     * Q-learning equation:
     *
     * Q(s,a) =
     * Q(s,a) +
     * alpha *
     * [reward +
     * gamma*max(Q(s',a')) -
     * Q(s,a)]
     */
    Q[state][action] =
        Q[state][action] +
        ALPHA *
        (
            reward +
            GAMMA * max_next -
            Q[state][action]
        );
}

float QLearning_GetValue(
    uint8_t state,
    uint8_t action
)
{
    return Q[state][action];
}
