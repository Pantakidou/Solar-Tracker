# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino embedded-firmware project for a dual-axis solar tracker. The device uses four LDR (Light Dependent Resistor) sensors to detect which direction has the most light and drives two servos to orient a solar panel toward the sun.

The entire firmware lives in one file: `TinyArduinofinalproject.ino`.

## Development Workflow

**Toolchain:** Arduino IDE (or Arduino CLI). There is no Makefile, CMake, or automated test suite — this is a hardware sketch uploaded directly to a microcontroller.

To compile and upload with Arduino CLI (targeting an ATtiny-class board):
```
arduino-cli compile --fqbn <board-fqbn> TinyArduinofinalproject.ino
arduino-cli upload  --fqbn <board-fqbn> -p <port> TinyArduinofinalproject.ino
```

To open in Arduino IDE, open `TinyArduinofinalproject.ino` directly.

## Hardware Pin Assignments

| Signal        | Pin |
|---------------|-----|
| Servo base (horizontal) | D4 |
| Servo top (vertical)    | D5 |
| LDR Top-Left   | A1 |
| LDR Top-Right  | A0 |
| LDR Bottom-Left | A3 |
| LDR Bottom-Right | A2 |

## Architecture

### Startup calibration (`calibrate_sensors`)
Runs for 10 seconds, continuously sampling all four LDRs to record per-sensor min/max values. Each sensor's threshold is set to `(max + min) / 2`. This auto-calibrates for ambient light conditions at power-on.

### Main control loop
Each iteration:
1. Reads all four LDRs.
2. Computes two differential signals:
   - **Vertical**: average of top sensors vs. average of bottom sensors.
   - **Horizontal**: average of left sensors vs. average of right sensors.
3. Applies a four-branch control law that increments/decrements `angle_servo_base` (horizontal) or `angle_servo_top` (vertical) by 1° per loop.
4. Clamps both angles to [0, 180].
5. Writes both servos.

### Custom servo PWM (`servo_write`)
The sketch does **not** use the Arduino `Servo` library. It manually generates a 50 Hz PWM signal by toggling a digital pin:
- Pulse width: `map(angle, 0, 180, 1000, 2000)` µs (HIGH phase).
- Low phase: `20000 − pulse_width` µs.

This means **each `servo_write` call blocks for ~20 ms**, so writing both servos costs ~40 ms per loop iteration.

## Known Code Issues

- **Line 69 bug:** The fourth branch of the horizontal control law reads:
  ```cpp
  angle_servo_base = angle_servo_top - 1;  // likely should be angle_servo_base - 1
  ```
  This cross-assigns the top servo angle to the base servo variable, causing incorrect horizontal tracking.

- **`servo_write` is blocking:** The loop has no `delay()` call, but the two sequential `servo_write` calls already consume ~40 ms each iteration. Any added logic should account for this timing.
