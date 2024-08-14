#include <SoftwareSerial.h>
SoftwareSerial bt(9,8); 
const int trigPin1 = 3;
const int echoPin1 = 2;
const int trigPin2 = 5;
const int echoPin2 = 4;
const int trigPin3 = 7;
const int echoPin3 = 6;
const int buzzer = A0;
const int vibrator=A1;


void setup() {
  // put your setup code here, to run once:
    pinMode(trigPin1, OUTPUT); 
    pinMode(echoPin1, INPUT); 
    pinMode(trigPin2, OUTPUT); 
    pinMode(echoPin2, INPUT); 
    pinMode(trigPin3, OUTPUT); 
    pinMode(echoPin3, INPUT); 
    pinMode(buzzer,OUTPUT);
    pinMode(vibrator,OUTPUT);
    Serial.begin(9600); 
    bt.begin(9600);

    
}

void loop() {
  /*if (bt.available() > 0)
  {
    char data = bt.read();
    Serial.println(data);*/
   
   long duration1, duration2, duration3, distance1, distance2, distance3;
  
  // First sensor
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  
  // Second sensor
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  
  // Third sensor
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  
 
  if (distance1 >50) 
  {
    Serial.println("Object detected in front");
    digitalWrite(buzzer,HIGH);
    digitalWrite(vibrator,HIGH);
    delay(2000);
    digitalWrite(buzzer,LOW);
    digitalWrite(vibrator,LOW);
  }
  if (distance2 >50) 
  {
    Serial.println("Object detected on the right");
    digitalWrite(buzzer,HIGH);
    digitalWrite(vibrator,HIGH);
    delay(2000);
    digitalWrite(buzzer,LOW);
    digitalWrite(vibrator,LOW);
  }
  if (distance3 >50)
  {
    Serial.println("Object detected on the left");
    digitalWrite(buzzer,HIGH);
    digitalWrite(vibrator,HIGH);
    delay(2000);
    digitalWrite(buzzer,LOW);
    digitalWrite(vibrator,LOW);
  }
  
  delay(1000); 
}
