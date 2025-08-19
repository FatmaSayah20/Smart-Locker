# 🔐 Smart Door Lock System with Keypad, LCD, Servo, and Ultrasonic Sensor

## 📖 Overview
This project implements a **Smart Door Lock System** using Arduino.  
The system secures access with a **password-protected keypad**, displays messages on an **LCD**, and controls a **servo motor** to open or close the door.  
It also integrates **ultrasonic sensing** to detect a person approaching and display a welcome message, plus **buzzer & LED alerts** for wrong attempts.

---

## 🚀 Features
- **Password Protection**:  
  - Default password: `1234`  
  - Enter `#` to confirm input  
  - `*` clears the input  

- **Password Change**:  
  - Enter master code `0000` followed by `#`  
  - Enter old password, then set and confirm a new password  

- **Security Lockout**:  
  - After 3 failed attempts, system locks for 10 seconds with buzzer alarm  

- **Automatic Door Control**:  
  - Door (servo) opens on correct password  
  - Auto-closes after 5 seconds  

- **Ultrasonic Welcome**:  
  - When someone approaches (within 30 cm), LCD shows a **"Welcome"** message  

---

## 🧩 Hardware Components
- Arduino board 
- 3x3 Keypad  
- Servo motor  
- 16x2 LCD display  
- Ultrasonic sensor 
- Buzzer  
- LED  
- Jumper wires & breadboard  


