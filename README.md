
---

## 🚦 How It Works

### Line Following

- **Black = LOW**, **White = HIGH** (from IR sensors)
- If both sensors are on black, the robot goes straight.
- If one sensor sees white, it adjusts to stay on the line.
- If both see white, it lost the line → starts recovery.

### Obstacle Avoidance

- If an object is detected <30cm:
  1. Robot turns right to exit line
  2. Moves forward beside the obstacle
  3. Turns left to re-align
  4. Moves forward and re-scans for the line

---

## 📖 Features

- Easy-to-follow logic structure
- Smart use of `millis()` for timeout-based recovery
- Full `Serial Monitor` feedback for debugging
- Code is modular and well-commented

---

## 🚀 Future Improvements

- Add a **third (center) IR sensor** for more accurate tracking
- Implement **PWM motor speed control** with `analogWrite()`
- Replace delays with **non-blocking state machines**
- Add **Bluetooth remote override** or **autonomous mapping**


---

## 🧠 Author

**Md Ratul Ahmed Alvi**  
Electronics Engineering Student  
University of Applied Sciences Hamm-Lippstadt
---
**Md Jubair Salehin Razin**  
Electronics Engineering Student  
University of Applied Sciences Hamm-Lippstadt
---
**Faysal Ahmed Tonmoy**  
Electronics Engineering Student  
University of Applied Sciences Hamm-Lippstadt
---
**Mahin**  
Electronics Engineering Student  
University of Applied Sciences Hamm-Lippstadt

