#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SoftwareSerial.h>
#include <time.h>

// WiFi Credentials
const char* ssid = "Indra5g";
const char* password = "snlt7249";

// Firebase Credentials
#define FIREBASE_HOST "https://petrolpump-123-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "kXuG8WZPy9OAGmmaeqUTwvEVy5dJgbDnSXKdjwa0"

// Firebase Initialization
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Serial Communication with Arduino Mega
SoftwareSerial MegaSerial(D2, D3); // D2 -> RX, D3 -> TX

void setup() {
    Serial.begin(9600);         
    MegaSerial.begin(9600);
    Serial.println("NodeMCU Initialized");

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure Firebase
    config.database_url = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    Serial.println("Connected to Firebase!");

    // Configure NTP Time Sync (IST UTC +5:30)
    configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("NTP Time Sync Configured");
}

void loop() {
    if (MegaSerial.available()) {
        String data = MegaSerial.readStringUntil('\n');
        data.trim();
        
        if (data.indexOf(',') == -1) {  // Authentication check
            String cardUID = data;
            Serial.println("Checking UID: " + cardUID);

            String path = "/petrolPump/" + cardUID + "/Balance";
            if (Firebase.getInt(firebaseData, path)) {
                int balance = firebaseData.intData();
                Serial.println("User Authenticated: Balance Rs." + String(balance));
                MegaSerial.println("OK," + String(balance));
            } else {
                Serial.println("User Not Found!");
                MegaSerial.println("ERROR");
            }
        } else {  // Transaction update
            int comma1 = data.indexOf(',');
            int comma2 = data.indexOf(',', comma1 + 1);
            if (comma1 != -1 && comma2 != -1) {
                String cardUID = data.substring(0, comma1);
                String amount = data.substring(comma1 + 1, comma2);
                String balance = data.substring(comma2 + 1);

                Serial.println("Updating Firebase:");
                Serial.println("UID: " + cardUID);
                Serial.println("Dispensed: " + amount + "ml");
                Serial.println("New Balance: Rs." + balance);

                String path = "/petrolPump/" + cardUID;
                String timestamp = getTimeStamp();

                if (Firebase.setInt(firebaseData, path + "/DispensedAmount", amount.toInt()) &&
                    Firebase.setInt(firebaseData, path + "/Balance", balance.toInt()) &&
                    Firebase.setString(firebaseData, path + "/LastTransaction", timestamp)) {
                    Serial.println("Transaction Updated!");
                } else {
                    Serial.println("Firebase Update Failed: " + firebaseData.errorReason());
                }
            }
        }
    }
}

String getTimeStamp() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return "N/A";
    }
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buffer);
}

