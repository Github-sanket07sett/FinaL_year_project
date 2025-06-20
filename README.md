# FinaL_year_project
# Smart Petrol Pump System
A smart petrol dispensing system using Arduino Mega, NodeMCU (ESP8266), RFID authentication, and Firebase. The system authenticates users using RFID cards, checks balance from Firebase, dispenses fuel accordingly, and updates the transaction details in real time.
## 📌 FEATURES
✅ RFID-based user authentication

💰 Real-time balance verification from Firebase

⛽ Fuel dispensing controlled via relay 

🔁 Live transaction updates pushed to Firebase

🕒 Timestamped transaction history

🖥️ OLED/I2C LCD support for on-site display

🔁 Bidirectional Serial Communication between Arduino Mega and NodeMCU
## 📲 Firebase Setup
1. Go to Firebase Console.
2. Create a new Realtime Database project.
3. Set rules to allow read/write access:
4. Copy the Database URL and Legacy Token and add it to your NodeMCU sketch.

![image](https://github.com/user-attachments/assets/79391657-0afa-44d6-9ab9-d4a39a5ddfa4)

## 🧠 System Workflow
1. User scans RFID card
2. NodeMCU checks Firebase for balance
3. If authenticated, Arduino receives "OK" with balance and allows fuel dispensing
4. Fuel is dispensed based on the balance and amount entered via keypad
5. Flow sensor measures dispensed amount
6. Firebase updates balance, dispensed amount, and timestamp


## Wiring Details
### Arduino Mega 2560
1. GND: Connected to GND of LCD, RFID, Relay, and ESP-8266.
2. 5V: Connected to VCC of LCD and Relay.
3. 3V3: Connected to VCC of RFID.
4. D53: Connected to SDA of RFID.
5. D52: Connected to SCK of RFID.
6. D51: Connected to MOSI of RFID.
7. D50: Connected to MISO of RFID.
8. D9 PWM: Connected to RST of RFID.
9. D11 PWM: Connected to IN of Relay.
10. A0-A3: Connected to R1-R4 of Keypad.
11. D7 PWM, D8 PWM, D4 PWM, D2 PWM: Connected to C1-C4 of Keypad.
12. D20/SDA: Connected to SDA of LCD.
13. D21/SCL: Connected to SCL of LCD.
14. D19/RX1: Connected to D3 of ESP-8266.
15. D18/TX1: Connected to pin2 of 2000 Ohm Resistor.
### ESP-8266 Controller
1. GND: Connected to GND of Arduino.
2. D2: Connected to pin2 of 1000 Ohm Resistor.
3. D3: Connected to D19/RX1 of Arduino.
### RFID-RC522
1. GND: Connected to GND of Arduino.
2. VCC (3.3V): Connected to 3V3 of Arduino.
3. SDA, SCK, MOSI, MISO, RST: Connected to respective pins on Arduino.
### 1 Channel Relay 5V
1. GND: Connected to GND of Arduino.
2. VCC: Connected to 5V of Arduino.
3. IN: Connected to D11 PWM of Arduino.
4. NC: Connected to Positive (+) of Water Pump.
5. COM: Connected to VCC of 12V Battery.
### Mini Diaphragm Water Pump
1. Positive (+): Connected to NC of Relay.
2. Negative (-): Connected to GND of 12V Battery.
### 12V Battery
1. VCC: Connected to COM of Relay.
2. GND: Connected to Negative (-) of Water Pump.
### 4X4 Membrane Matrix Keypad
1. R1-R4: Connected to A0-A3 of Arduino.
2. C1-C4: Connected to D7 PWM, D8 PWM, D4 PWM, D2 PWM of Arduino.
### LCD 16x2 attached i2c
1. GND: Connected to GND of Arduino.
2. VCC: Connected to 5V of Arduino.
3. SDA: Connected to D20/SDA of Arduino.
4. SCL: Connected to D21/SCL of Arduino.
### Resistors
1. 1000 Ohms: Connected between D2 of ESP-8266 and pin1 of 2000 Ohm Resistor.
2. 2000 Ohms: Connected between pin2 of 1000 Ohm Resistor and D18/TX1 of Arduino.
## Block Diagram
![DIAGRAM](https://github.com/user-attachments/assets/e089e80a-5879-41f7-8812-7657ee983df8)

## Circuit Diagram
![circuit_image (1)](https://github.com/user-attachments/assets/b86166c6-c3c1-4297-bece-cfec2d244672)
## Flowchart
![flowcahrt](https://github.com/user-attachments/assets/9a67e609-d545-4794-86b7-9c0d4a663246)
## Project Architecture
![image](https://github.com/user-attachments/assets/6b353e57-6835-40fc-9801-dbea92768ef2)


## 🛠️ Requirements
1. Arduino IDE 1.8+
2. Libraries:
   1. FirebaseESP8266.h
   2. ESP8266WiFi.h
   3. MFRC522.h
   4. Wire.h
   5. LiquidCrystal_I2C.h or U8g2lib.h
