#include<LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 2, 3, 4, 5);

#define Speed A1
#define buz 13
#define rf A0

#define m1 8
#define m2 9
#define en 10

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome"); delay(1000);

  pinMode(buz, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(rf, INPUT);
}
int vs = 0;
int speed_data;
int zone_st = 0;
void loop()
{
  int rf_data = digitalRead(rf);
  speed_data = analogRead(Speed);
  speed_data = map(speed_data, 0, 1024, 0, 200);
  int en_speed = map(speed_data, 0, 200, 0, 120);
  digitalWrite(buz,LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zone:" + String(rf_data));
  lcd.setCursor(0, 1);
  lcd.print("Speed:" + String(en_speed));


  if (rf_data == 1)
  {
    zone_st = 1;
    lcd.setCursor(8, 0);
    lcd.print("Public");
    digitalWrite(buz,HIGH);
    delay(100);
  }
  else
  {
    zone_st = 0;
    lcd.setCursor(8, 0);
    lcd.print("Clear ");
  }



  if (en_speed > 0 && en_speed < 5)
  {
    vs = 0;
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    analogWrite(en, 0);
  }
  else
  {
    vs = 1;
  }

  if (vs == 1)
  {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    if (zone_st == 0)
    {
      analogWrite(en, speed_data);
      if (en_speed > 0 && en_speed <= 40)
      {
        lcd.setCursor(10, 1);
        lcd.print("Low ");
      }

      if (en_speed > 40 && en_speed <= 60)
      {
        lcd.setCursor(10, 1);
        lcd.print("Eco ");
      }

      if (en_speed > 60 && en_speed <= 120)
      {
        lcd.setCursor(10, 1);
        lcd.print("High ");
      }
    }

    if (zone_st == 1)
    {
      if (en_speed > 0 && en_speed <= 5)
      {
        analogWrite(en, 0);
      }
      else if (en_speed > 5 && en_speed < 60)
      {
        analogWrite(en, speed_data);
      }
      else
      {
        analogWrite(en, 50);
        lcd.setCursor(0, 1);
        lcd.print("Speed:30 ");
        lcd.setCursor(10, 1);
        lcd.print("LOW ");
      }
    }
  }
  delay(250);

}
