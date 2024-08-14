#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A1, A0, 8, 9}; //connect to the column pinouts of the keypad


const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int motor = 12;
int buz = 13;
char key;
String p1 = "06004F910AD2";
String p2 = "2F0087BDB9AC";
String p3 = "5900D493E1FF";
String p4 = "2F00875CE81C";

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(motor, OUTPUT);
  pinMode(buz, OUTPUT);
  digitalWrite(buz, HIGH);  // Motor on
  digitalWrite(motor, LOW);    // motor off
  lcd.setCursor(0, 0);
  lcd.print(" Unmaned Petrol");
  lcd.setCursor(5, 1);
  lcd.print("  Bunk");
  delay(1000);                       // wait for a second
  digitalWrite(buz, LOW);  // Motor on
  // char key = keypad.getKey();
}
char pass[4];
char amt[5];
int a1 = 1000;
int a2 = 1000;
int a3 = 1000;

void loop()
{
  lcd.clear();
  lcd.print("Plz Show UR Card");
  digitalWrite(buz, LOW);
  if (Serial.available() > 0)
  {
    String x = Serial.readString();
    Serial.println(x);
    if (x == p1)
    {
      digitalWrite(buz, LOW);
      lcd.setCursor(2, 1);
      lcd.print("Card Valid");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:");
      lcd.setCursor(6, 1);
      key = keypad.getKey();
      delay(1000);
      int i = 0;
      while (i < 4)
      {
        key = keypad.getKey();
        if (key)
        {
          pass[i] = key;
          lcd.print(key);
          i++;
          delay(10);
        }
      }
      pass[i] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entered Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:" + String(pass));
      delay(1000);
      if (String(pass) == "1234")
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A/c:Rajesh");
        lcd.setCursor(0, 1);
        lcd.print("Balance:" + String(a1));  delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Plz enter Amount:");
        lcd.setCursor(0, 1);
        lcd.print("Amount:");
        delay(1000);
        int j = 0;
        key = keypad.getKey();
        while (key != '*')
        {
          key = keypad.getKey();
          if (key)
          {
            amt[j] = key;
            lcd.print(key);
            j++;
            delay(10);
          }
        }
        amt[j - 1] = '\0';
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Entered Amount");
        lcd.setCursor(0, 1);
        int a = atoi(amt);
        lcd.print("Rupees:" + String(a) + "/-"); delay(1000);
        if (a1 >=  a)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Payment Done!");
          delay(1000);
          a1 = a1 - a;
          digitalWrite(motor, HIGH); delay((100 * a)); digitalWrite(motor, LOW);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Insufficent");
          lcd.setCursor(0, 1);
          lcd.print("        Amount");
          delay(1000);
        }
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Password");
        delay(1000);
      }
    }
    else if (x == p2)
    {
      digitalWrite(buz, LOW);
      lcd.setCursor(2, 1);
      lcd.print("Card Valid");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:");
      lcd.setCursor(6, 1);
      key = keypad.getKey();
      delay(1000);
      int i = 0;
      while (i < 4)
      {
        key = keypad.getKey();
        if (key)
        {
          pass[i] = key;
          lcd.print(key);
          i++;
          delay(10);
        }
      }
      pass[i] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entered Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:" + String(pass));
      delay(1000);
      if (String(pass) == "1111")
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A/c:Vijay");
        lcd.setCursor(0, 1);
        lcd.print("Balance:" + String(a2));  delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Plz enter Amount:");
        lcd.setCursor(0, 1);
        lcd.print("Amount:");
        delay(1000);
        int j = 0;
        key = keypad.getKey();
        while (key != '*')
        {
          key = keypad.getKey();
          if (key)
          {
            amt[j] = key;
            lcd.print(key);
            j++;
            delay(10);
          }
        }
        amt[j - 1] = '\0';
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Entered Amount");
        lcd.setCursor(0, 1);
        int a = atoi(amt);
        lcd.print("Rupees:" + String(a) + "/-"); delay(1000);
        if (a2 >=  a)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Payment Done!");
          delay(1000);
          a2 = a2 - a;
          digitalWrite(motor, HIGH); delay((100 * a)); digitalWrite(motor, LOW);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Insufficent");
          lcd.setCursor(0, 1);
          lcd.print("        Amount");
          delay(1000);
        }
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Password");
        delay(1000);
      }
    }
    else if (x == p3)
    {
      digitalWrite(buz, LOW);
      lcd.setCursor(2, 1);
      lcd.print("Card Valid");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:");
      lcd.setCursor(6, 1);
      key = keypad.getKey();
      delay(1000);
      int i = 0;
      while (i < 4)
      {
        key = keypad.getKey();
        if (key)
        {
          pass[i] = key;
          lcd.print(key);
          i++;
          delay(10);
        }
      }
      pass[i] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entered Password");
      lcd.setCursor(0, 1);
      lcd.print("pswd:" + String(pass));
      delay(1000);
      if (String(pass) == "2222")
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A/c:Prem kumar");
        lcd.setCursor(0, 1);
        lcd.print("Balance:" + String(a3));  delay(3000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Plz enter Amount:");
        lcd.setCursor(0, 1);
        lcd.print("Amount:");
        delay(1000);
        int j = 0;
        key = keypad.getKey();
        while (key != '*')
        {
          key = keypad.getKey();
          if (key)
          {
            amt[j] = key;
            lcd.print(key);
            j++;
            delay(10);
          }
        }
        amt[j - 1] = '\0';
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Entered Amount");
        lcd.setCursor(0, 1);
        int a = atoi(amt);
        lcd.print("Rupees:" + String(a) + "/-"); delay(1000);
        if (a3 >=  a)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Payment Done!");
          delay(1000);
          a3 = a3 - a;
          digitalWrite(motor, HIGH); delay((100 * a)); digitalWrite(motor, LOW);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Insufficent");
          lcd.setCursor(0, 1);
          lcd.print("        Amount");
          delay(1000);
        }
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Password");
        delay(1000);
      }
    }
    else
    {
      digitalWrite(buz, HIGH);
      lcd.setCursor(1, 1);
      lcd.print("Card Not Valid");
      delay(1000);
    }
  }
  delay(100);
}
