#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define NodeMCU Serial1  // Use Serial1 for Mega TX/RX (Pins 18, 19)

// RFID Setup
#define SS_PIN 53
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad Setup
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = { {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'} };
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {7, 8, 4, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Relay Setup
#define RELAY_PIN 11
#define FLOW_RATE_MS_PER_ML 40 

struct CardData {
  String uid;
  int balance;
};
CardData cards[100]; 
int numCards = 0;

void setup() {
  Serial.begin(9600);
  NodeMCU.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Initializing...");

  SPI.begin();
  rfid.PCD_Init();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  lcd.clear();
  lcd.print("Ready for RFID");
}

void loop() {
  int cardIndex = authenticateUser();
  if (cardIndex != -1) {
    startDispensing(cardIndex);
  }
  delay(2000);
}

int authenticateUser() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String cardUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      cardUID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX);
    }
    cardUID.toUpperCase();

    Serial.print("Card UID: ");
    Serial.println(cardUID);
    lcd.clear();

    int cardIndex = -1;
    for (int i = 0; i < numCards; i++) {
      if (cards[i].uid == cardUID) {
        cardIndex = i;
        break;
      }
    }

    if (cardIndex == -1) {
      if (numCards < 100) {
        cards[numCards].uid = cardUID;
        cards[numCards].balance = 10000;
        cardIndex = numCards;
        numCards++;
        
        Serial.println("New Card Registered with 10000ml Balance");
        lcd.print("New Card Added");
        lcd.setCursor(0, 1);
        lcd.print("Bal: 10000ml");
        delay(2000);
      } else {
        lcd.print("Memory Full!");
        delay(2000);
        return -1;
      }
    }

    lcd.clear();
    lcd.print("UID: ");
    lcd.print(cardUID.substring(0, 6));
    lcd.setCursor(0, 1);
    lcd.print("Bal: ");
    lcd.print(cards[cardIndex].balance);
    lcd.print("ml");
    delay(2000);

    return cardIndex;
  }
  return -1;
}

void startDispensing(int cardIndex) {
  lcd.clear();
  lcd.print("Enter ml:");

  String amountStr = "";
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') break;
      else if (key == '*') amountStr = "";
      else amountStr += key;
      lcd.clear();
      lcd.print("Amount: " + amountStr + "ml");
    }
  }

  int fuelAmountML = amountStr.toInt();
  
  if (fuelAmountML <= 0) {
    lcd.clear();
    lcd.print("Invalid Input!");
    delay(2000);
    return;
  }
  
  if (fuelAmountML > cards[cardIndex].balance) {
    lcd.clear();
    lcd.print("Insufficient Funds!");
    delay(2000);
    return;
  }
  
  cards[cardIndex].balance -= fuelAmountML;

  lcd.clear();
  lcd.print("Dispensing:");
  lcd.setCursor(0, 1);
  lcd.print(fuelAmountML);
  lcd.print("ml");
  delay(2000);

  digitalWrite(RELAY_PIN, HIGH);
  delay(fuelAmountML * FLOW_RATE_MS_PER_ML);
  digitalWrite(RELAY_PIN, LOW); 
  
  delay(500);
  
  digitalWrite(RELAY_PIN, HIGH);
  delay(200);
  digitalWrite(RELAY_PIN, LOW);

  lcd.clear();
  lcd.print("UID: ");
  lcd.print(cards[cardIndex].uid.substring(0, 6));
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(fuelAmountML);
  lcd.print("ml B:");
  lcd.print(cards[cardIndex].balance);
  lcd.print("ml");
  delay(4000);

  String dataToSend = cards[cardIndex].uid + "," + String(fuelAmountML) + "," + String(cards[cardIndex].balance);
  NodeMCU.println(dataToSend);
  Serial.println("DISPENSED: " + dataToSend);

  rfid.PICC_HaltA();
  lcd.clear();
  lcd.print("Ready for RFID");
}

