# Event-Driven ECG Intelligence on STM32

## Problem Statement
Continuous ECG monitoring on wearable devices consumes significant computational
resources and power. This project implements a hierarchical, event-driven ECG
intelligence system on the STM32F401CCU6 that escalates computation only when
the signal demands it, keeping the MCU asleep most of the time.

## Hardware
- STM32F401CCU6 (Black Pill)
- ST-LINK V2
- USB-to-TTL cable
- AD8232 ECG sensor with electrodes
- DS18B20 temperature sensor
- Breadboard + M-M jumper wires

## Architecture
- **Tier 0 (always-on gate):** Timer + ADC + DMA sample the ECG at 250 Hz with
  the CPU asleep. Lead-off detection filters out invalid signal.
- **Tier 1 (per-beat):** Filtering and R-peak detection using CMSIS-DSP,
  producing heart rate and RR intervals.
- **Tier 2 (triggered on anomaly):** Beat morphology classification
  (fuzzy logic / decision tree) against a per-patient template.
- **Tier 3 (rare):** Deeper analysis or raw signal logging, triggered only
  on sustained anomalies.
- A Q-learning agent, trained offline on MIT-BIH, governs escalation between
  tiers to balance detection sensitivity against energy cost.

## Repository Structure
firmware/   STM32CubeIDE project (acquisition + signal processing)
python/     Live plotting, offline analysis, Q-learning training
docs/       Problem statement, wiring guide, architecture diagram
data/       Recorded ECG samples for offline testing

## Status
- [x] ADC + DMA + Timer acquisition chain
- [x] UART streaming to laptop
- [x] Live plotting in Python
- [ ] Tier 1 R-peak detection (CMSIS-DSP)
- [ ] Tier 2 morphology classifier
- [ ] Q-learning escalation policy
- [ ] Digital twin for simulated biological parameters

## Key Code Snippet — Event-Driven Acquisition

The core of Tier 0 is interrupt-driven, not polled: the timer and DMA move
ADC samples on their own, and the CPU only wakes up twice per buffer cycle.

```c
// Timer2 triggers the ADC at 250 Hz; DMA moves samples with zero CPU polling
#define ECG_BUF 250
uint16_t adcBuf[ECG_BUF];
volatile uint8_t halfReady = 0, fullReady = 0;

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* h) { halfReady = 1; }
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* h)     { fullReady = 1; }

// in main():
HAL_TIM_Base_Start(&htim2);
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuf, ECG_BUF);

// in the while(1) loop — only runs when a buffer half is actually full:
if (halfReady) {
    halfReady = 0;
    for (int i = 0; i < ECG_BUF/2; i++) {
        int n = sprintf(line, "%u\n", adcBuf[i]);
        HAL_UART_Transmit(&huart1, (uint8_t*)line, n, 10);
    }
}
```

The full firmware project is in [`firmware/`](firmware/); the Python live-plotting
script that consumes this stream is in [`python/live_plot.py`](python/live_plot.py).

## How to Run

1. Flash `firmware/ecg` onto the STM32F401 via STM32CubeIDE + ST-LINK
2. Wire the AD8232 and USB-to-TTL as described in [`docs/wiring.md`](docs/wiring.md)
3. `pip install -r python/requirements.txt`
4. `python3 python/live_plot.py` (update the `PORT` variable to match your machine)

## Team
- Akanksha Kumari — Software
- Nishayini K — Software
- Theertha Santosh — Hardware
