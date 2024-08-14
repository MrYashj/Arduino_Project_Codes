#include<SoftwareSerial.h>
SoftwareSerial bt(8, 9);
#define m1 4
#define m2 5
#define m3 6
#define m4 7

#define m5 A0
#define m6 A1
#define m7 A2
#define m8 A3
#define relay A4


void setup()
{
  Serial.begin(9600);
  bt.begin(9600);
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

  bt.print("1-forward, 2-backward, 3-left, 4-right, 5-stop, u-up, d-down, C-close, o-open");
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

    if (data == 'u' || data == 'U')
    {
      up();
    }

    if (data == 'd' || data == 'D')
    {
      down();
    }

    if (data == 'c' || data == 'C')
    {
      Close();
    }

    if (data == 'o' || data == 'O')
    {
      Open();
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


void up()
{
  digitalWrite(m6, HIGH);
  digitalWrite(m5, LOW);
  delay(1000);
  digitalWrite(m6, LOW);
  digitalWrite(m5, LOW);
}

void down()
{
  digitalWrite(m6, LOW);
  digitalWrite(m5, HIGH);
  delay(1000);
  digitalWrite(m6, LOW);
  digitalWrite(m5, LOW);
}

void Open()
{
  digitalWrite(m8, HIGH);
  digitalWrite(m7, LOW);
  delay(250);
  digitalWrite(m8, LOW);
  digitalWrite(m7, LOW);
}

void Close()
{
  digitalWrite(m8, LOW);
  digitalWrite(m7, HIGH);
  delay(250);
  digitalWrite(m8, LOW);
  digitalWrite(m7, LOW);
}
