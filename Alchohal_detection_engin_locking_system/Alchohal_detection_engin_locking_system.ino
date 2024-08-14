#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define Key 8
#define buz 13

#define relay A1
#define sensor A0
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println("WELCOME");
  pinMode(buz, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(Key, INPUT_PULLUP);
  stop_vehicle();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("WELCOME"); delay(2000);
}
int Key_st = 0;
int count = 0;
void loop()
{
  if (digitalRead(Key) == LOW)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alcohol:"); lcd.print(analogRead(sensor)); delay(200);

    if (analogRead(sensor) < 300)
    {
      count = 0;
      start_vehicle();
      lcd.setCursor(0, 1);
      lcd.print("Vehicle Running.."); delay(250);
    }
    else
    {
      Serial.println("Time:" + String(count));
      count++; digitalWrite(buz, HIGH);
      delay(500);
      digitalWrite(buz, LOW); delay(500);
      if (count == 3)
      {
        while (digitalRead(Key) == LOW)
        {
          count = 0;
          stop_vehicle();
          digitalWrite(buz, HIGH);
          lcd.setCursor(0, 1);
          lcd.print("Alcohol Found"); delay(250);
        }
      }
    }
  }
  if (digitalRead(Key) == HIGH)
  {
    digitalWrite(buz, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turn ON the KEY");
    lcd.setCursor(0, 1);
    lcd.print("To Start Vehicle");
    stop_vehicle(); delay(300);
  }
}

void start_vehicle()
{
  digitalWrite(relay, LOW);
}
void stop_vehicle()
{
  digitalWrite(relay,HIGH);
}
