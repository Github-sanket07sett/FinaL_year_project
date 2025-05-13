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

## ✅ Future Enhancements
1.Integrate mobile app for users to recharge balance
2. Add OTP-based confirmation before dispensing
3. Introduce admin dashboard for monitoring fuel sales
## 🛠️ Requirements
1. Arduino IDE 1.8+
2. Libraries:
   1. FirebaseESP8266.h
   2. ESP8266WiFi.h
   3. MFRC522.h
   4. Wire.h
   5. LiquidCrystal_I2C.h or U8g2lib.h
