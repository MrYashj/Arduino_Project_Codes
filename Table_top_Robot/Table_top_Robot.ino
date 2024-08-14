#define in1 A0
#define in2 A3
#define in3 A2
#define in4 A1

#define ir1 6
#define ir2 7

void setup()
{
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
}

void loop()
{
  Serial.println("IR1:" + String(digitalRead(ir1)) + "   " + "IR2:" + String(digitalRead(ir2)));
  if ((digitalRead(ir1) == LOW) && (digitalRead(ir2) == LOW))
  {
    forward();
  }

  if ((digitalRead(ir1) == LOW) && (digitalRead(ir2) == HIGH))
  {
    left();
  }

   if ((digitalRead(ir1) == HIGH) && (digitalRead(ir2) == LOW))
  {
    right();
  }

  if ((digitalRead(ir1) == HIGH) && (digitalRead(ir2) == HIGH))
  {
    Stop(); delay(1000);
    backward(); delay(2000);
    left(); delay(1000);
  }
}


void forward()
{
  Serial.println("forward");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward()
{
  Serial.println("Backward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left()
{
  Serial.println("Left");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right()
{
  Serial.println("Right");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop()
{
  Serial.println("Stop");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4,LOW);
}
