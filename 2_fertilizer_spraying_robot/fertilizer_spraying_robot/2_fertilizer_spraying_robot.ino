#include<SoftwareSerial.h>
SoftwareSerial bt(8, 9);
#define m1 2
#define m2 3
#define m3 4
#define m4 5

#define relay1 6
#define relay2 7


void setup()
{
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  bt.print("1-forward, 2-backward, 3-left, 4-right, 5-stop, 6-pump on, 7-pump off,8-Plough on, 9-plough off ");
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
      digitalWrite(relay1, LOW);
    }

    if (data == '7')
    {
      digitalWrite(relay1, HIGH);
    }

    if (data == '8')
    {
      digitalWrite(relay2, LOW);
    }

    if (data == '9')
    {
      digitalWrite(relay2, HIGH);
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
