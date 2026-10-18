# Moving Average Filter

This project was developed as part of a **Signals and Systems** course. The goal of the workshop was to apply fundamental concepts of signal processing to a practical hardware-software project.

The project focuses primarily on **audio and music signals**. A C-based program running on a **Texas Instruments Tiva™ C Series TM4C123G LaunchPad** analyzes an input signal, while an electronic circuit with multiple LEDs provides a visual representation of the processed signal. The software was developed using **Texas Instruments Code Composer Studio (CCS)**.

## Project Overview

The project demonstrates how signal-processing techniques can be implemented in C on a microcontroller and combined with electronic hardware for real-time signal visualization.

A music track is played using an external device and fed into an electronic circuit on a **breadboard**. The resulting signal is acquired and interpreted by the **TM4C123G microcontroller** on the LaunchPad, which is responsible for processing the signal.

The first part of the workshop focuses on **volume measurement using a Moving Average Filter**. The current volume of the music signal is continuously analyzed, and the measured volume is displayed using **eight LEDs**, creating a simple visual representation of the signal's amplitude.

## Moving Average Filter

A **Moving Average Filter (MAF)** is used to smooth the input signal by calculating the average of a fixed number of consecutive samples. This reduces rapid fluctuations and provides a more stable representation of the signal.

The standard output \(y[n]\) of a simple moving average filter with a window length of \(N\) is defined as:

$$
y[n] = \frac{1}{N}\sum_{k=0}^{N-1}x[n-k]
$$

Where:

- \(x[n]\) is the current input sample.
- \(y[n]\) is the filtered output sample.
- \(N\) is the number of points (taps) in the moving average window.

In this project, the ADC values are **squared before being stored in the ring buffer**. The moving average is then calculated over these squared values to obtain a more stable representation of the signal's volume.

## How It Works

The signal-processing workflow can be summarized as follows:

1. A music track is played using an external audio device.
2. The audio signal is fed into the electronic circuit built on a **breadboard**.
3. The signal is acquired and interpreted by the **TM4C123G microcontroller** on the LaunchPad.
4. The C program applies a **Moving Average Filter** to smooth the signal and obtain a more stable representation of its volume.
5. The resulting volume level is mapped to eight different levels.
6. The corresponding number of LEDs is illuminated according to the measured volume.

This allows changes in the audio signal to be observed directly through the LED display.

## Hardware

- **Texas Instruments Tiva™ C Series TM4C123G LaunchPad** — development board featuring the TM4C123G microcontroller
- **8 LEDs** for volume visualization
- Breadboard-based electronic circuit
- External audio device for music playback
- Electronic components for signal input and LED control

## Software & Technologies

- **C** — signal processing and embedded software
- **Texas Instruments Code Composer Studio (CCS)** — development environment
- **TivaWare™** — Texas Instruments software libraries for the TM4C123G microcontroller
- **Moving Average Filter** — smoothing and volume estimation
- **Digital Signal Processing** — analysis of audio signals
- **Embedded Systems** — real-time signal processing on a microcontroller
- **ADC, Timers & Interrupts** — signal acquisition and periodic processing
- **GPIO / LED Control** — visual representation of the processed signal

## Learning Objectives

The project provided practical experience with:

- Analyzing and processing real-world audio signals
- Applying filtering techniques to audio signals
- Implementing signal-processing algorithms in C
- Developing software for a microcontroller
- Working with the **Tiva™ C Series TM4C123G LaunchPad**
- Using **Code Composer Studio** for embedded software development
- Interfacing a microcontroller with external electronic hardware
- Working with ADCs, timers, interrupts, and GPIO
- Converting numerical signal information into a physical output
- Combining software and electronic hardware
- Understanding the practical application of concepts from Signals and Systems

