#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DHT.h>

// Definitions for sensors and relay
#define IR_SENSOR_PIN 3
#define FLAME_SENSOR_PIN A0
#define MQ2_SENSOR_PIN A1
#define DHT_PIN 4
#define RELAY_PIN 2
#define BUZZER_PIN 13

#define DHT_TYPE DHT11

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

#define EEPROM_START_ADDRESS 0 // Starting address in EEPROM

// Initialize the LCD with the I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize SoftwareSerial for GSM module communication
SoftwareSerial gsm(8, 9); // RX, TX

void setup() {
  // Start Serial Monitor
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

   lcd.print("Industrial Safety");
  lcd.setCursor(0,1);
  lcd.print("  Automation  ");
  delay(2000);
  lcd.clear();
  
  // Initialize GSM module
  gsm.begin(9600);

  // Initialize sensors
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(MQ2_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);
  dht.begin();

  // Check if there is a stored mobile number in EEPROM
  String storedNumber = readNumberFromEEPROM();
  if (storedNumber.length() > 0) {
    // Display stored number on LCD
    displayNumberOnLCD(storedNumber);
    
    // Send SMS
    sendSMS(storedNumber, "WELCOME");
  } else {
    Serial.println("Enter mobile number:");
  }
}

void loop() {
  Serial.println("Please enter '#' to change the number then press ENTER ");
  if (Serial.available()) {
    delay(100); // Wait for data to fully arrive
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newline or spaces
    
    if (input == "#") {
      Serial.println("Enter new mobile number:");
      while (!Serial.available()) {
        // Wait for user to enter new number
      }
      delay(100); // Wait for data to fully arrive
      String newNumber = Serial.readStringUntil('\n');
      newNumber.trim(); // Remove any trailing newline or spaces
      
      // Store new mobile number in EEPROM
      storeNumberInEEPROM(newNumber);
      
      // Display stored number on LCD
      displayNumberOnLCD(newNumber);
      
      // Send SMS
      sendSMS(newNumber, "WELCOME");
    }
  }

  // Read sensor values
  int irValue = digitalRead(IR_SENSOR_PIN);
  int flameValue = digitalRead(FLAME_SENSOR_PIN);
  int mq2Value = analogRead(MQ2_SENSOR_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Display sensor values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temperature);
  lcd.setCursor(8, 0);
  lcd.print("IR:"); 
  lcd.setCursor(11, 0);
  lcd.print(irValue);
  lcd.setCursor(0, 1);
  lcd.print("Fire:");
  lcd.setCursor(5, 1); 
  lcd.print(flameValue);
  lcd.setCursor(8, 1);
  lcd.print("MQ2:");
  lcd.setCursor(12, 1);
  lcd.print(mq2Value);

  // If IR sensor detects a person
  if (irValue == LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    String storedNumber = readNumberFromEEPROM();
    if (storedNumber.length() > 0) {
      String message = "Person Detected! T: " + String(temperature) + "C, H: " + String(humidity) + "%, Flame: " + String(flameValue) + ", MQ2: " + String(mq2Value);
      if (sendSMS(storedNumber, message)) {
        displayMessageOnLCD("MSG SENT SUCCESS");
      }
    }
  }

  if (flameValue == LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    String storedNumber = readNumberFromEEPROM();
    if (storedNumber.length() > 0) {
      String message = "Fire Detected! T: " + String(temperature) + "C, H: " + String(humidity) + "%, Flame: " + String(flameValue) + ", MQ2: " + String(mq2Value);
      if (sendSMS(storedNumber, message)) {
        displayMessageOnLCD("MSG SENT SUCCESS");
      }
    }
  }

  if (mq2Value > 600) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
    String storedNumber = readNumberFromEEPROM();
    if (storedNumber.length() > 0) {
      String message = "GAS Detected! T: " + String(temperature) + "C, H: " + String(humidity) + "%, Flame: " + String(flameValue) + ", MQ2: " + String(mq2Value);
      if (sendSMS(storedNumber, message)) {
        displayMessageOnLCD("MSG SENT SUCCESS");
      }
    }
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }

  if (temperature >37) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
    String storedNumber = readNumberFromEEPROM();
    if (storedNumber.length() > 0) {
      String message = "Temperature High! T: " + String(temperature) + "C, H: " + String(humidity) + "%, Flame: " + String(flameValue) + ", MQ2: " + String(mq2Value);
      if (sendSMS(storedNumber, message)) {
        displayMessageOnLCD("MSG SENT SUCCESS");
      }
    }
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }

  delay(500); // Wait for seconds before the next loop
}

void storeNumberInEEPROM(String number) {
  int len = number.length();
  
  // Write the length of the number
  EEPROM.write(EEPROM_START_ADDRESS, len);
  
  // Write the number into EEPROM
  for (int i = 0; i < len; i++) {
    EEPROM.write(EEPROM_START_ADDRESS + 1 + i, number[i]);
  }
}

String readNumberFromEEPROM() {
  int len = EEPROM.read(EEPROM_START_ADDRESS);
  
  // If the length is 0 or not within reasonable limits, return an empty string
  if (len <= 0 || len > 15) {
    return "";
  }

  char number[len + 1]; // +1 for null terminator
  
  for (int i = 0; i < len; i++) {
    number[i] = EEPROM.read(EEPROM_START_ADDRESS + 1 + i);
  }
  
  number[len] = '\0'; // Null terminator
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
  delay(2000); // Display message for 2 seconds
}

bool sendSMS(String number, String message) {
  gsm.print("AT+CMGF=1\r"); // Set SMS mode to text
  delay(1000);
  gsm.print("AT+CMGS=\"");
  gsm.print(number);
  gsm.print("\"\r");
  delay(1000);
  gsm.print(message);
  delay(1000);
  gsm.write(26); // Ctrl+Z to send the SMS
  delay(5000); // Wait for the message to be sent

  // Check if SMS was sent successfully
  if (gsm.find("OK")) {
    return true;
  } else {
    return false;
  }
}
