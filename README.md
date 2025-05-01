
#  Intelligent Line Follower & Obstacle Avoidance Robot

##  System Overview

This autonomous robot is designed for seamless **line tracking** and **obstacle avoidance**. It combines smart logic, modular code, and real-time feedback to perform reliable navigation in dynamic environments.

---

## 🛣 Line Following Logic

- **Sensor Input**:  
  - **Black surface** → `LOW`  
  - **White surface** → `HIGH`
  
- **Behavioral Responses**:
  - ➤ **Both sensors detect black** → Move straight
  - ➤ **One sensor detects white** → Adjust direction to stay on the line
  - ➤ **Both sensors detect white** → Line lost → Initiates recovery mode

---

##  Obstacle Avoidance Protocol

When an object is detected within **30 cm** using an ultrasonic sensor:

1. ➤ **Right Turn** – Exit the line path
2. ➤ **Forward Motion** – Navigate alongside the obstacle
3. ➤ **Left Turn** – Re-align with the original path
4. ➤ **Forward Motion** – Search and lock back onto the line

---

## ⚙️ Key Features


- ⏱️ **Non-Blocking Timers** – Efficient use of `millis()` for delay-free recovery  
- 🪛 **Debug with Ease** – Real-time diagnostics via `Serial Monitor`  
- 📦 **Modular Codebase** – Clear, reusable, and well-documented functions  

---

## 👨‍💻 Contributors

| Name                    | Title                            | Institution                                           |
|-------------------------|----------------------------------|--------------------------------------------------------|
| **Md Ratul Ahmed Alvi** | Electronics Engineering Student  | Hochschule Hamm-Lippstadt         |
| **Md Jubair Salehin Razin** | Electronics Engineering Student  | Hochschule Hamm-Lippstadt         |
| **Faysal Ahmed Tonmoy** | Electronics Engineering Student  | Hochschule Hamm-Lippstadt         |
| **Mahin**               | Electronics Engineering Student  | Hochschule Hamm-Lippstadt         |

---

## 📎 Final Notes

This project is a fusion of **practical embedded systems**, **real-time sensing**, and **autonomous logic**—crafted with clarity and purpose for students, makers, and future robotics engineers.
