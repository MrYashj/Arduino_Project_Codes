#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Definitions for IR sensor and relay
#define securitykey 12
#define ignitionkey 3
#define RELAY_PIN 2
#define BUZZER_PIN 13

#define EEPROM_START_ADDRESS 0 // Starting address in EEPROM

// ADXL335 pins
#define X_PIN A2
#define Y_PIN A3

// Initialize the LCD with the I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize SoftwareSerial for GSM module and GPS module communication
SoftwareSerial gsm(10, 11); // RX, TX for GSM
SoftwareSerial gps(8, 9); // RX, TX for GPS

TinyGPSPlus gpsParser;

unsigned long previousMillis = 0;
const long interval = 500;  // Interval at which to read switches
const int tiltThreshold = 300; // Adjust the threshold based on your calibration

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print(" Vehicle Theft ");
  lcd.setCursor(0, 1);
  lcd.print("      And     ");
  delay(2000);
  lcd.clear();
  lcd.print("    Accident  ");
  lcd.setCursor(0, 1);
  lcd.print("Detection System");
  delay(2000);
  lcd.clear();

  gsm.begin(9600);
  gps.begin(9600);

  pinMode(securitykey, INPUT_PULLUP);
  pinMode(ignitionkey, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);

  String storedNumber = readNumberFromEEPROM();
  if (storedNumber.length() > 0) {
    displayNumberOnLCD(storedNumber);
    sendSMS(storedNumber, "WELCOME");
  } else {
    Serial.println("Enter mobile number:");
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int security = digitalRead(securitykey);
    int ignition = digitalRead(ignitionkey);

    Serial.println("Press # to change mobile number");
    Serial.print("Security key:");
    Serial.println(security);
    Serial.print("Ignition key:");
    Serial.println(ignition);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Skey:");
    lcd.setCursor(5, 0);
    lcd.print(security);
    lcd.setCursor(8, 0);
    lcd.print("Vkey:");
    lcd.setCursor(13, 0);
    lcd.print(ignition);

    lcd.setCursor(0, 1);
    lcd.print("Turn key 2 start");

    if (security == 0) {
      if (ignition == 0) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Vehicle started");
        digitalWrite(RELAY_PIN, LOW);
        String storedNumber = readNumberFromEEPROM();
        if (storedNumber.length() > 0) {
          String gpsData = getGPSData();
          String message = "Vehicle Started By Authorized person. Location: " + gpsData;
          if (sendSMS(storedNumber, message)) {
            displayMessageOnLCD("MSG SENT SUCCESS");
          }
        }
      } else {
        digitalWrite(RELAY_PIN, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Vehicle stopped  ");
      }
    }

    if (ignition == 0 && security == 1) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Theft detected");
      digitalWrite(BUZZER_PIN, HIGH);
      String storedNumber = readNumberFromEEPROM();
      if (storedNumber.length() > 0) {
        String gpsData = getGPSData();
        String message = "Theft Detected! Location: " + gpsData;
        if (sendSMS(storedNumber, message)) {
          displayMessageOnLCD("MSG SENT SUCCESS");
        }
      }
    }

    // Read ADXL335 accelerometer data
    int xValue = analogRead(X_PIN);
    int yValue = analogRead(Y_PIN);

    Serial.print("X:");
    Serial.println(xValue);
    Serial.print("Y:");
    Serial.println(yValue);

    if ((xValue > 450 && yValue < 390)) // || (xValue < 310 && yValue > 370)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vehicle Crashed ");
      String storedNumber = readNumberFromEEPROM();
      if (storedNumber.length() > 0) {
        String gpsData = getGPSData();
        String message = "Vehicle Crashed Left Side! Location: " + gpsData;
        if (sendSMS(storedNumber, message)) {
          displayMessageOnLCD("MSG SENT SUCCESS");
        }
      }
    }
    if ((xValue < 330 && yValue <= 400)) // || (xValue < 310 && yValue > 370)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Vehicle Crashed ");
      String storedNumber = readNumberFromEEPROM();
      if (storedNumber.length() > 0) {
        String gpsData = getGPSData();
        String message = "Vehicle Crashed Right Side! Location: " + gpsData;
        if (sendSMS(storedNumber, message)) {
          displayMessageOnLCD("MSG SENT SUCCESS");
        }
      }
    }
  }

  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "#") {
      Serial.println("Enter new mobile number:");
      while (!Serial.available()) {
        // Wait for user to enter new number
      }
      String newNumber = Serial.readStringUntil('\n');
      newNumber.trim();
      storeNumberInEEPROM(newNumber);
      displayNumberOnLCD(newNumber);
      sendSMS(newNumber, "WELCOME");
    }
  }

  // Update GPS data
  while (gps.available() > 0) {
    gpsParser.encode(gps.read());
  }
}

void storeNumberInEEPROM(String number) {
  int len = number.length();
  EEPROM.write(EEPROM_START_ADDRESS, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(EEPROM_START_ADDRESS + 1 + i, number[i]);
  }
}

String readNumberFromEEPROM() {
  int len = EEPROM.read(EEPROM_START_ADDRESS);
  if (len <= 0 || len > 15) {
    return "";
  }

  char number[len + 1];
  for (int i = 0; i < len; i++) {
    number[i] = EEPROM.read(EEPROM_START_ADDRESS + 1 + i);
  }

  number[len] = '\0';
  return String(number);
}

void displayNumberOnLCD(String number) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stored Number:");
  lcd.setCursor(0, 1);
  lcd.print(number);
}

void displayMessageOnLCD(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  delay(3000); // Increase delay to ensure message is displayed
}

bool sendSMS(String number, String message) {
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.print("AT+CMGS=\"");
  gsm.print(number);
  gsm.print("\"\r");
  delay(1000);
  gsm.print(message);
  delay(1000);
  gsm.write(26); // Ctrl+Z to send the SMS
  delay(3000);

  if (gsm.find("OK")) {
    return true;
  } else {
    return false;
  }
}

String getGPSData() {
  if (gpsParser.location.isUpdated()) {
    double latitude = gpsParser.location.lat();
    double longitude = gpsParser.location.lng();
    String googleMapsLink = "https://www.google.com/maps/search/?api=1&query=" + String(latitude, 6) + "," + String(longitude, 6);
    return googleMapsLink;
  } else {
    return "No GPS Data";
  }
}
