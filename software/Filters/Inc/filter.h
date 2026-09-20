#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

void Filter_Init(void);

float Filter_Process(float input);

uint8_t Filter_IsSignalGood(void);

#endif
