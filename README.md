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

## Team
- Akanksha Kumari — Software
- Nishayini K — Software
- Theertha Santosh — Hardware