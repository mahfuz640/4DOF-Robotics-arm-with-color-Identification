# 🤖 4DOF Robotic Arm with Color Identification

A 4 Degree-of-Freedom (4DOF) robotic arm controlled by Arduino, capable of **detecting colors** and sorting objects accordingly.  
The project combines **mechanical control** with **computer vision** (color detection using sensors or a camera).

---

## ✨ Features
- 4DOF robotic arm (Base, Shoulder, Elbow, Gripper).  
- **Color identification** using a TCS3200/TCS34725 color sensor or camera.  
- Automatic object sorting based on color.  
- Manual + automatic control modes.  
- Serial Monitor debug output.  

---

## 🛠️ Hardware Requirements
- Arduino UNO / Mega / ESP32  
- 4 × Servo motors (SG90/MG995/servo compatible)  
- TCS3200 or TCS34725 color sensor (or OpenCV + camera if using Raspberry Pi/PC)  
- Robotic arm frame (3D-printed / acrylic / DIY)  
- Breadboard + jumper wires  
- Power supply (5V, 2A recommended)  

---

## ⚡ Circuit Connections
| Component         | Arduino Pin |
|-------------------|-------------|
| Servo (Base)      | D9  |
| Servo (Shoulder)  | D10 |
| Servo (Elbow)     | D11 |
| Servo (Gripper)   | D6  |
| Color Sensor S0   | D2  |
| Color Sensor S1   | D3  |
| Color Sensor S2   | D4  |
| Color Sensor S3   | D5  |
| Color Sensor OUT  | D7  |

*(Adjust pins as per your wiring)*

---

## 📂 Code Overview
- **Servo Control**: Each servo is mapped to an axis (Base, Shoulder, Elbow, Gripper).  
- **Color Detection**: Reads RGB values from the sensor.  
- **Sorting Logic**:  
  - If RED → Move arm to drop at Position A.  
  - If GREEN → Move arm to drop at Position B.  
  - If BLUE → Move arm to drop at Position C.  
- Uses Serial Monitor for debugging detected colors and servo positions.  

---

## ▶️ How to Use
1. Assemble the robotic arm and connect components as per the table.  
2. Upload `robotic_arm_color.ino` to Arduino.  
3. Place objects in front of the **color sensor**.  
4. Arm will detect color → pick → place into the correct bin.  

---

## 🔧 Customization
- Change **servo pin mapping** in the code:
  ```cpp
  Servo baseServo.attach(9);
  Servo shoulderServo.attach(10);
  Servo elbowServo.attach(11);
  Servo gripperServo.attach(6);
