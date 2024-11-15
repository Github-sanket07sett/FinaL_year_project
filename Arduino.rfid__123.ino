#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

String MasterCard = "668A8F8";
String tagId = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened (for ATMEGA32U4-based Arduinos)
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);             // Optional delay
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  tagId = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
  tagId += String(mfrc522.uid.uidByte[i], HEX);
}
tagId.toUpperCase(); // Ensure uppercase
Serial.println("Scanned UID: " + tagId); // Print UID to check for exact match

// Check against MasterCard
if (tagId == MasterCard) {
  Serial.println("Access Granted");
} else {
  Serial.println("Access Denied");
}


  delay(2000); // Wait a bit before scanning the next card
  Serial.println("Access Control");

  // Halt PICC
  mfrc522.PICC_HaltA();
}
