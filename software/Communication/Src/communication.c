#include "communication.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static char tx_buffer[64];

void Communication_Init(void)
{
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

void Communication_SendECG(uint16_t adc_value)
{
    int length;

    length = snprintf(
        tx_buffer,
        sizeof(tx_buffer),
        "%u\r\n",
        adc_value
    );

    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)tx_buffer,
        length,
        100
    );
}

void Communication_SendHeartRate(float heart_rate)
{
    int length;

    length = snprintf(
        tx_buffer,
        sizeof(tx_buffer),
        "HR: %.1f BPM\r\n",
        heart_rate
    );

    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)tx_buffer,
        length,
        100
    );
}

void Communication_SendRR(float rr_interval)
{
    int length;

    length = snprintf(
        tx_buffer,
        sizeof(tx_buffer),
        "RR: %.3f s\r\n",
        rr_interval
    );

    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)tx_buffer,
        length,
        100
    );
}
