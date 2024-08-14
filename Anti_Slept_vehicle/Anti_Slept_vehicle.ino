#define Key 7
#define buz 8
#define red1 2
#define vib 5
#define relay 9
#define sensor 6
void setup()
{
  Serial.begin(9600);
  Serial.println("WELCOME");
  pinMode(red1, OUTPUT);
  pinMode(vib, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(Key, INPUT_PULLUP);
  pinMode(sensor, INPUT);
  digitalWrite(vib, HIGH);
  digitalWrite(buz, HIGH);
}
int Key_st = 0;
int count = 0;
void loop()
{
  if (digitalRead(Key) == LOW)
  {
    Serial.println("Key ON");
    if (digitalRead(sensor) == HIGH)
    {
      count = 0;
      start_vehicle();
      digitalWrite(vib, LOW);
      digitalWrite(buz, LOW);
      Serial.println("Vehicle ON");
    }
    else
    {
      Serial.println("Eye Closed");
      Serial.println("Time:" + String(count));
      count++; digitalWrite(buz, HIGH);
      digitalWrite(vib, HIGH);
          digitalWrite(red1,HIGH);
      delay(500);
      digitalWrite(buz, LOW);
      digitalWrite(vib, LOW);
          digitalWrite(red1,HIGH); delay(500);
      if (count == 5)
      {
        while (digitalRead(Key) == LOW)
        {
          stop_vehicle();
          digitalWrite(buz, HIGH);
          digitalWrite(red1,HIGH);
          digitalWrite(vib, HIGH);
          delay(300);
          digitalWrite(buz, LOW);
          digitalWrite(red1, LOW);
          digitalWrite(vib, LOW);
          delay(300);
          Serial.println("Vehicle OFF");
        }
      }
    }
  }
  if (digitalRead(Key) == HIGH)
  {
    digitalWrite(red1, HIGH);
    digitalWrite(vib, LOW);
    digitalWrite(buz, LOW);
    stop_vehicle();
    Serial.println("Key off ");
  }
}

void start_vehicle()
{
  digitalWrite(relay, HIGH);
}
void stop_vehicle()
{
  digitalWrite(relay, LOW);
}
