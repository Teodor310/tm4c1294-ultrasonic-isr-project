# TM4C1294 Ultrasonic Distance Measurement System

## Overview
This project implements an interrupt-driven ultrasonic distance measurement system using the TM4C1294NCPDT microcontroller. It uses GPIO and Timer1A interrupts to detect echo pulses from an ultrasonic sensor and calculates distance based on the time-of-flight.

## Features
- Interrupt-driven timing (no busy-waiting)
- GPIO port D interrupt for echo pulse detection
- Timer1A setup for pulse timing
- Button interrupt on Port J to measure the time of buttons being pressed
- Basic console output via `printf` (assumes UART is set up externally)
- Distance displayed in centimeters

## Hardware Requirements
- TM4C1294NCPDT microcontroller
- HC-SR04 Ultrasonic Distance Sensor
- Debug console (UART0)

## Software Features
- ISR-based GPIO handler for echo pulse edge detection
- Timer1A configured in one-shot mode
- Delay using software (microsecond scale)
- Calculation:
  Speed of sound is approximated at ~343 m/s

## File Structure
- `main.c` – main loop and core configuration
- `int_handler.c` – interrupt service routines for Port D and Port J
- `int_handler.h` – header file with definitions and globals

 ## 🚦 Ultrasonic Measurement Flow

This project uses interrupt-driven software to measure distance with an ultrasonic sensor (e.g., HC-SR04). The process is as follows:

### Flow Summary

1. **Send Trigger**  
   The microcontroller sends a `10 µs` high pulse to the trigger pin.

2. **Emit Ultrasonic Wave**  
   The ultrasonic module emits the wave and sets the echo pin high.

3. **ISR (Rising Edge)**  
   An interrupt service routine (ISR) starts the timer when the echo pin goes high.

4. **Wait for Echo Return**  
   The ultrasonic wave reflects off an object and returns to the module.

5. **ISR (Falling Edge)**  
   The ISR stops the timer and calculates the distance when the echo pin goes low.

6. **Repeat**  
   The process repeats for the next measurement cycle.


## Building
Use [TI Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) with TivaWare libraries. Include the `inc/` folder for hardware register definitions.

## Known Issues
- Current implementation uses software delay for trigger pulses (could be replaced with hardware PWM)
- Direction control logic placeholder (`original` / `reverse`) not yet used functionally

## License
MIT License

## Author
Created as part of coursework at Hamburg University of Applied Sciences.
