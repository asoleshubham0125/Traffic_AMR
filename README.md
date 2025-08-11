# Traffic_AMR – Automated Mobility Robot for Traffic Barrier System

## 📌 Overview
The **Traffic_AMR** project is an **Automated Mobility Robot** designed to control and automate traffic barrier operations.  
It uses ultrasonic sensors for obstacle detection, motor drivers for movement control, and LED indicators for motion status.  
This system can be controlled via serial commands (`R`, `L`, `S`) and includes safety features to prevent collisions.

---

## 🚀 Features
- **Left & Right Movement** with obstacle avoidance.
- **Ultrasonic Sensor Integration** for distance measurement.
- **LED Indicators:**
  - **Yellow LED**: Warning before movement.
  - **Red LED**: Indicates motion in progress.
- **Buzzer Alert** (optional, can be enabled in code).
- **Serial Command Interface**:
  - `R` → Move Forward
  - `L` → Move Backward
  - `S` → Stop
- **Safety Timeout** to automatically stop after a set duration.

---

## 🛠 Components
Refer to [`Component Req and Cost Analysis.pdf`](docs/Component%20Req%20and%20Cost%20Analysis.pdf) for the detailed Bill of Materials (BOM).

**Key Components:**
- PIC Microcontroller (with MCC-generated configuration)
- Ultrasonic Sensors (HC-SR04 or equivalent)
- Motor Driver (L298N or equivalent)
- DC Motors
- LEDs (Red, Yellow)
- Buzzer (optional)
- Serial Communication Module (EUSART1)
- Power Supply

---

## ⚙️ Hardware Setup
![Circuit Diagram](simulation/circuit.png)

- **Ultrasonic Sensors**:
  - Sensor 1: TRIG → RC0, ECHO → RC1
  - Sensor 2: TRIG → RC2, ECHO → RC3
- **LEDs**:
  - Red LED → RC4
  - Yellow LED → RC5
- **Motors**:
  - Controlled via `LATD` pins for forward/backward motion.
- **Buzzer**:
  - Optional, can be connected to RB0.

---

## 📂 Project Structure
