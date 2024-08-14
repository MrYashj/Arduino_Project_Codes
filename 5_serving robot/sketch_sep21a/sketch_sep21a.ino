#include<SoftwareSerial.h>
SoftwareSerial bt(8, 9);//rx,tx
#define m1 2
#define m2 3
#define m3 4
#define m4 5



void setup()
{
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
//  pinMode(buz, OUTPUT);
}

void loop()
{
  if (bt.available() > 0)
  {
    char data = bt.read();
    Serial.println(data);

    if (data == '1')
    {
      Serial.println("forward");
      bt.println("forward");
      forward();
    }

    if (data == '2')
    {
      Serial.println("backward");
      backward();
    }

    if (data == '3')
    {
      Serial.println("right");
      right();
    }

    if (data == '4')
    {
      Serial.println("left");
      left();
    }
    if (data == '5')
    {
      Serial.println("stop");
      Stop();
    }
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
