#include<SoftwareSerial.h>
SoftwareSerial bt(10, 11);
#define m1 2
#define m2 3
#define m3 4
#define m4 5

#define m5 6
#define m6 7
#define m7 8
#define m8 9
#define relay A0
#define sensor A5
#define buz 13


void setup()
{
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(sensor, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);

  pinMode(m5, OUTPUT);
  pinMode(m6, OUTPUT);
  pinMode(m7, OUTPUT);
  pinMode(m8, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  digitalWrite(buz, LOW);
  
  bt.print("1-forward, 2-backward, 3-left, 4-right, 5-stop, 6-light on, 7-light off, A-cone on, B-cone off, C-cutter on, D-cutter off");
}

void loop()
{
  if (bt.available() > 0)
  {
    char data = bt.read();
    Serial.println(data);

    if (data == '1')
    {
      bt.println("forward");
      forward();
    }

    if (data == '2')
    {
      backward();
    }

    if (data == '3')
    {
      left();
    }

    if (data == '4')
    {
      right();
    }
    if (data == '5')
    {
      Stop();
    }

    if (data == '6')
    {
      digitalWrite(relay, LOW);
    }

    if (data == '7')
    {
      digitalWrite(relay, HIGH);
    }

    if (data == 'a' || data == 'A')
    {
      cone_on();
    }

    if (data == 'b' || data == 'B')
    {
      cone_off();
    }

    if (data == 'c' || data == 'C')
    {
      cutter_on();
    }

    if (data == 'd' || data == 'D')
    {
      cutter_off();
    }
  }

  Serial.println(analogRead(sensor));
  delay(100);
  if(analogRead(sensor)<500)
  {
    Stop();
    digitalWrite(buz,HIGH);
    delay(50);
    digitalWrite(buz,LOW);
    delay(50);
    }
}



void forward()
{
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  digitalWrite(m3, HIGH);
  digitalWrite(m4, LOW);
}

void backward()
{
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
  digitalWrite(m3, LOW);
  digitalWrite(m4, HIGH);
}

void left()
{
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, HIGH);
}

void right()
{
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
  digitalWrite(m3, HIGH);
  digitalWrite(m4, LOW);
}

void Stop()
{
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, LOW);
}


void cone_on()
{
  digitalWrite(m5, HIGH);
  digitalWrite(m6, LOW);
}

void cone_off()
{
  digitalWrite(m5, LOW);
  digitalWrite(m6, LOW);
}

void cutter_on()
{
  digitalWrite(m7, HIGH);
  digitalWrite(m8, LOW);
}

void cutter_off()
{
  digitalWrite(m7, LOW);
  digitalWrite(m8, LOW);
}
