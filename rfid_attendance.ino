#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>

//-----------------------------------------
#define RST_PIN  D3
#define SS_PIN   D4
#define BUZZER   D2
//-----------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;  
MFRC522::StatusCode status;      
//-----------------------------------------
int blockNum = 2;  
byte bufferLen = 18;
byte readBlockData[18];
//-----------------------------------------
String card_holder_name;
const String sheet_url = "https://script.google.com/macros/s/AKfycbwR_y67hOlMxX-6xA_KOHrcc5n-eMLFyp9ousjqM9zniN4Xpz88jyp0gUv6b_C-a1p_Vg/exec?name=";

// Temporarily using insecure connection for debugging (set to true if SSL is causing issues)
const bool USE_INSECURE = true;

// Define the fingerprint if needed for a secure connection
const uint8_t fingerprint[32] = {
    0x0b, 0x1f, 0xdc, 0xa0, 0x4f, 0x29, 0xe5, 0x60, 
    0x38, 0x5f, 0xb9, 0x5e, 0xf5, 0xe6, 0x49, 0x9a, 
    0x82, 0xb6, 0x76, 0x2b, 0xac, 0xd6, 0x3e, 0x49, 
    0xa2, 0x89, 0x76, 0x51, 0x3d, 0x12, 0xc1, 0xa4
};
//-----------------------------------------
#define WIFI_SSID "Manuvsmancity_4G"
#define WIFI_PASSWORD "tamashapiku"
//-----------------------------------------

void setup()
{
  Serial.begin(9600);
  Serial.println();
  
  // Wi-Fi Connectivity
  Serial.print("Connecting to AP...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Set BUZZER as OUTPUT
  pinMode(BUZZER, OUTPUT);
  
  // Initialize SPI bus
  SPI.begin();
}

void loop()
{
  mfrc522.PCD_Init();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) { 
    return;
  }

  Serial.println("\nReading last data from RFID...");
  ReadDataFromBlock(blockNum, readBlockData);

  Serial.print("Last data in RFID block ");
  Serial.print(blockNum);
  Serial.print(" --> ");
  for (int j = 0; j < 16; j++) {
    Serial.write(readBlockData[j]);
  }
  Serial.println();

  digitalWrite(BUZZER, HIGH); delay(200); digitalWrite(BUZZER, LOW); delay(200);
  
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    if (USE_INSECURE) {
      client->setInsecure();  // Use insecure mode
    } else {
      client->setFingerprint(fingerprint);  // Secure mode with fingerprint
    }

    card_holder_name = sheet_url + String((char*)readBlockData);
    card_holder_name.trim();
    Serial.println("Requesting URL: " + card_holder_name);

    HTTPClient https;
    Serial.println("[HTTPS] begin...");

    if (https.begin(*client, card_holder_name)) {
      Serial.println("[HTTPS] GET...");
      int httpCode = https.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
          String payload = https.getString();
          Serial.println("Response payload:");
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.println("[HTTPS] Unable to connect");
    }

    delay(1000);
  }
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed for Read: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    Serial.println("Authentication success");
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else {
    Serial.println("Block was read successfully");  
  }
}
