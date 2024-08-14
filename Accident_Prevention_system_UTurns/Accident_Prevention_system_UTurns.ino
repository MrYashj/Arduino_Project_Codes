#define trigPin1 8  // Trigger pin
#define echoPin1 9  // Echo pin
#define trigPin2 10  // Trigger pin
#define echoPin2 11  // Echo pin

#define left_red A2
#define left_green A3

#define right_red A0
#define right_green A1
#define buz 13

long duration1;
int distance1;
long duration2;
int distance2;

void setup()
{
  Serial.begin(9600);    // Initialize serial communication
  pinMode(trigPin1, OUTPUT);    // Set the trigger and echo pins as input/output
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);    // Set the trigger and echo pins as input/output
  pinMode(echoPin2, INPUT);
  pinMode(left_red, OUTPUT);
  pinMode(left_green, OUTPUT);
  pinMode(right_red, OUTPUT);
  pinMode(right_green, OUTPUT);
  pinMode(buz,OUTPUT);

  digitalWrite(left_red, LOW);
  digitalWrite(left_green, LOW);
  digitalWrite(right_red, LOW);
  digitalWrite(right_green, LOW);
  digitalWrite(left_green, HIGH);
  digitalWrite(right_green, HIGH);
  digitalWrite(buz, HIGH); delay(1000);
  digitalWrite(buz, LOW); delay(1000);
   
}
int r_signal = 0;
int l_signal = 0;
int r_count = 0;
int l_count = 0;
void loop()
{
  
  get_distance_left();
  get_distance_right();
  delay(500);
  digitalWrite(buz, LOW);
  if (distance1 < 10)
  {
    r_count  = 10;
    r_signal = 1;
  }
  if (distance2 < 10)
  {
    l_count = 10;
    l_signal = 1;
  }

  if (r_signal == 1)
  {
    if (r_count > 0)
    {
      digitalWrite(buz, HIGH);
      digitalWrite(right_red, HIGH);
      digitalWrite(right_green, LOW);
    }
  }
  else
  {
    digitalWrite(right_red, LOW);
    digitalWrite(right_green, HIGH);
  }

  if (l_signal == 1)
  {
    if (l_count > 0)
    {
      digitalWrite(buz, HIGH);
      digitalWrite(left_red, HIGH);
      digitalWrite(left_green, LOW);
    }
  }
  else
  {
    digitalWrite(left_red, LOW);
    digitalWrite(left_green, HIGH);
  }

  r_count--;
  l_count--;
  if(r_count<=0)
  {
    r_count=0;
    r_signal=0;
  }
  if(l_count<=0)
  {
    l_count=0;
    l_signal=0;
  }
}

void get_distance_left()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Read the duration of the pulse on the echo pin
  duration1 = pulseIn(echoPin1, HIGH);

  // Calculate the distance in centimeters
  distance1 = duration1 * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Left: ");
  Serial.print(distance1);
  Serial.println(" cm");
}


void get_distance_right()
{
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Read the duration of the pulse on the echo pin
  duration2 = pulseIn(echoPin2, HIGH);

  // Calculate the distance in centimeters
  distance2 = duration2 * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Right: ");
  Serial.print(distance2);
  Serial.println(" cm");
}
