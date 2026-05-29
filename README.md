# 🔐 Door Lock System – Dual ECU Embedded Project
## 📌 Project Overview

This project implements a secure door lock system using two TM4C123GH6PM (Tiva-C) microcontrollers:

HMI_ECU (Human–Machine Interface ECU)
Handles user interaction (keypad, LCD, LEDs, potentiometer).

Control_ECU
Handles security logic, EEPROM storage, motor control, and buzzer alarms.

The two ECUs communicate via UART using a simple command-based protocol.

## 🎥 Demo Video

[![Watch the Demo](https://img.shields.io/badge/Watch-Demo_Video-red?style=for-the-badge&logo=youtube)](https://drive.google.com/file/d/1K1BoKKxZdrg9O2ZNQbJ_Yw8FRdLq5nKG/view?usp=sharing)


## 🧩 System Features

First-time password setup

Secure password verification

Password stored in EEPROM (non-volatile)

3-attempt lockout with buzzer alarm

Motor-controlled door locking/unlocking

Adjustable auto-lock timeout (5–30 seconds)

Fully layered architecture (MCAL / HAL / APP)

Unit, integration, and system-level testing

Resource usage analysis (ROM, RAM, Stack)

Coding-standard compliance documentation

## 🧱 Software Layering Model
### 1️⃣ MCAL (Microcontroller Abstraction Layer)

Direct register access

Hardware-specific drivers

Examples:

uart1.c

systick.c

eeprom.c

### 2️⃣ HAL (Hardware Abstraction Layer)

Device-oriented drivers

Independent of application logic

Examples:

Motor, Buzzer

LCD, Keypad, LEDs, ADC

### 3️⃣ Application Layer (APP)

Business logic

System flow and state control

Examples:

Password validation

Door operation

Lockout handling

## 🧪 Testing Strategy
### ✔ Unit Testing

Individual drivers tested in isolation

Examples:

Motor direction control

EEPROM read/write

UART TX/RX

### ✔ Integration Testing

UART ↔ EEPROM

UART ↔ Motor

UART ↔ Buzzer

### ✔ System Testing

First-boot password creation

Correct / incorrect login

3-attempt lockout behavior

Auto-lock timing accuracy
