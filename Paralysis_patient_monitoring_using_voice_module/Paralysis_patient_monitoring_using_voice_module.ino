#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
int mem=A5;

#include "DHT.h"

#define DHTPIN A4// what pin we're connected to



// Uncomment whatever type you're using!
  #define DHTTYPE DHT11   // DHT 11

  DHT dht(DHTPIN, DHTTYPE);


static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;
char st = 0;
float gps_lat, gps_lng;

TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
void gps_read();
void displayInfo();
char z = 0;

char gsm[50], irq_gsm = 0, ms, msg[20];
char mobilenum[] = "9002140892 ";
void gsm_init(void);
void gsm_send_num_mesg(char *num1, char *msg1);
void read_msg();
char gsm_clear_msg(char num);
void gpsread();
void btn_pressed();
//#define finger A1
void clear_gsm() //variable clear in gsm
{
  unsigned char uc;
  for (uc = 0; uc < 50; uc++)
    gsm[uc] = '\0';
}

void read_gsm(void) // reading data from gsm
{
  while (Serial.available())
  {
    gsm[irq_gsm] = Serial.read();
    irq_gsm++;
    if (irq_gsm > 50)
      irq_gsm = 50;
  }
}

int in1=21;
int in2=20;
int in3=19;
int in4=18;
int in5=17;
int in6=16;

const int flexPin1 = A0;
const int flexPin2 = A1;
const int flexPin3 = A2;
const int flexPin4 = A3;




int flexValue1;
int flexValue2;
int flexValue3;
int flexValue4;
int memval;



const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Start serial communication
  Serial.begin(9600);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(in5,OUTPUT);
  pinMode(in6,OUTPUT);

  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH); 
  digitalWrite(in3,HIGH);
  digitalWrite(in4,HIGH);
  digitalWrite(in5,HIGH);
  digitalWrite(in6,HIGH);

  pinMode(flexPin1,INPUT);
  pinMode(flexPin2,INPUT);
  pinMode(flexPin3,INPUT);
  pinMode(flexPin4,INPUT);
  pinMode(mem,INPUT);

  // Initialize LCD
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0,0); lcd.print("SIGN CONVERSION");
  delay(2000);
  //lcd.clear();
 // lcd.setCursor(7,0); lcd.print("F2:");
 // lcd.setCursor(0,1); lcd.print("F3:");
 // lcd.setCursor(7,1); lcd.print("F4:");

   gsm_init();
  gsm_clear_msg(1);
  gsm_clear_msg(2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NO SIGN CONVERSION");

  Serial.println((String)mobilenum);//comment this line if ur not using in built Number

  if (st == 1)
  {
    st = 0;
  }

}
unsigned int i;

void loop() {
  // Read the values from the flex sensors
  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  flexValue3 = analogRead(flexPin3);
  flexValue4 = analogRead(flexPin4);
  memval=analogRead(mem);

  Serial.print("mem val: ");
  Serial.println(memval);
  delay(1000);
 
  Serial.print("Flex Sensor 1: ");
  Serial.println(flexValue1);
  delay(1000);
  
  Serial.print("Flex Sensor 2: ");
  Serial.println(flexValue2);
  delay(1000);
  Serial.print("Flex Sensor 3: ");
  Serial.println(flexValue3);
  delay(1000);
  Serial.print("Flex Sensor 4: ");
  Serial.println(flexValue4);
  delay(1000);
  // Check which sensor is activated
  if (flexValue1 > 850)
    {
    digitalWrite(in1,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("give me food");
     gsm_send_num_mesg("9002140892", "Please give me food");
    //gsm_send_num_mesg("8096296923", "Please give me food");
    //gsm_send_num_mesg("9963951606", "Please give me food");
    //gsm_send_num_mesg("9963951606", "Please give me food");
     lcd.setCursor(0,0);
    lcd.print("give me food");
    delay(7000);
    lcd.setCursor(0,0);
    lcd.print("NO SIGN CONVERSION");
    }
    else
    {
      digitalWrite(in1,HIGH);
    }
    if (flexValue2 > 800) 
    {
      digitalWrite(in2,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("give me water");
      gsm_send_num_mesg("9002140892", "Please give me water");
      //gsm_send_num_mesg("8096296923", "Please give me water");
      //gsm_send_num_mesg("9963951606", "Please give me water");
      lcd.setCursor(0,0);
    lcd.print("give me Water");
    delay(7000);
    lcd.setCursor(0,0);
    lcd.print("NO SIGN CONVERSION");
    // lcd.setCursor(0,1);
   // lcd.print(f);
    }
    else
    {
      digitalWrite(in2,HIGH);
    }
    if (flexValue3 > 800) 
    {
      digitalWrite(in3,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("take 2 washroom");
      gsm_send_num_mesg("9002140892", "Please Take to me washroom");
      //gsm_send_num_mesg("8096296923", "Please Take to me washroom");
      //gsm_send_num_mesg("9963951606", "Please Take to me washroom");
      lcd.setCursor(0,0);
    lcd.print("take 2 washroom");
    delay(7000);
    lcd.setCursor(0,0);
    lcd.print("NO SIGN CONVERSION");
    // lcd.setCursor(0,1);
   // lcd.print(f);
     } 
  else
  {
    digitalWrite(in3,HIGH); 
  }
  if (flexValue4 > 800) 
  {
    digitalWrite(in4,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("emergency");
    gsm_send_num_mesg("9002140892", "EMERGENCY ALERT");
    // gsm_send_num_mesg("8096296923", "EMERGENCY ALERT");
     // gsm_send_num_mesg("9963951606", "EMERGENCY ALERT");
    lcd.setCursor(0,0);
    lcd.print("EMERGENCY ALERT");
    delay(7000);
    lcd.setCursor(0,0);
    lcd.print("NO SIGN CONVERSION");
   // lcd.setCursor(0,1);
    //lcd.print(f);
  }
  else
  {
    digitalWrite(in4,HIGH);
  }

   if (memval>390)//(map(memval, 250, 1023, 0, 400))
  {
    digitalWrite(in6,LOW);
    lcd.clear();
    gsm_send_num_mesg("9002140892", "PATIENT FELLDOWN");
   //gsm_send_num_mesg("8096296923", "PATIENT FELLDOWN");
    //gsm_send_num_mesg("9963951606", "PATIENT FELLDOWN");
   // lcd.setCursor(0,0);
   // lcd.print("PATIENT FALL");
    
    lcd.setCursor(0,0);
    lcd.print("PATIENT FALL");
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("NO SIGN CONVERSION");
  }
  else
  {
    digitalWrite(in6,HIGH);
  }
    
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
    float  t= dht.readTemperature();
     float f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

   // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F");
  lcd.setCursor(0,1);
  lcd.print("temp: ");
  lcd.setCursor(6,1);
  lcd.print("    ");
   lcd.setCursor(6,1);
  lcd.print(f);


  if(f>100)
  { 
    digitalWrite(in5,LOW);
     gsm_send_num_mesg("9002140892", "FEVER ALERT");
    // gsm_send_num_mesg("8096296923", "FEVER ALERT");
    // gsm_send_num_mesg("9963951606", "FEVER ALERT");
     lcd.setCursor(1,0);
     lcd.print(f);
  }
  else
  {
    digitalWrite(in5,HIGH);
  }
  
}
  void btn_pressed()
{

  gps_read();
  if (st == 1)
  {
    st = 0;
    // break;
  }

  //gsm_send_num_mesg(mobilenum, "Track Fault:", "http://www.google.co.in/maps/place/");
  //gsm_send_num_mesg("6281432369", "Track Fault:", "http://www.google.co.in/maps/place/");
  irq_gsm = 0; clear_gsm();

}

void gps_read()
{
  if (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
      displayInfo();
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.print("Pls Check Ur GPS Connections");

  }
}



void displayInfo()
{
  Serial.print("GPS READING..");

  if (gps.location.isValid())
  {
    gps_lat = gps.location.lat();
    gps_lng = gps.location.lng();

    Serial.print(String(gps_lat, 6));
    Serial.print(F(","));
    Serial.println(String(gps_lng, 6));
    st = 1;
  }
  else
  {
    Serial.println("INVALID VALUES");
    st = 0;
  }

}



void gsm_init(void)///gsm working or not checking
{
  unsigned int ui_t1;
  char E_count = 0;

gsminit:
  lcd.clear(); lcd.print("GSM INIT...");
  Serial.print("ATE0"); Serial.println(); delay(500); irq_gsm = 0;

AT:
  Serial.println(); delay(100);
  Serial.print("AT"); Serial.println();
  lcd.clear(); lcd.print("AT: ");
  delay(500);
  irq_gsm = 0;
  read_gsm();

  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    Serial.println(); delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();         lcd.print(" AT FAILED");
    Serial.println(); delay(1000);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto AT;
  }


  clear_gsm(); E_count = 0;


  Serial.print("ATE0"); irq_gsm = 0; Serial.println();; Serial.println(); delay(500);

  clear_gsm(); E_count = 0;
ATE0:
  Serial.print("ATE0"); irq_gsm = 0; Serial.println();; Serial.println();
  lcd.clear(); lcd.print("ATE0: ");
  delay(500);
  read_gsm();
  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    Serial.println(); delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();         lcd.print("ATE0 FAILED");
    Serial.println(); delay(500);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATE0;
  }

  clear_gsm(); E_count = 0;

ATCMGF:
  Serial.print("AT+CMGF=1"); Serial.println();
  lcd.clear(); lcd.print("TEXT MODE: ");
  irq_gsm = 0; Serial.println();
  delay(500);
  read_gsm();
  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    Serial.println(); delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();         lcd.print("TEXT MODE");
    lcd.setCursor(0, 1); lcd.print("     NOT OK");
    Serial.println();  delay(1000);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCMGF;
  }

  clear_gsm(); E_count = 0;

ATCPIN:
  Serial.print("AT+CPIN?");  Serial.println();;
  irq_gsm = 0;  Serial.println();
  delay(500);
  read_gsm();

  if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'P' && gsm[5] == 'I' && gsm[6] == 'N' && gsm[7] == ':' && gsm[8] == ' ' && gsm[9] == 'R')
  {
    lcd.clear();         lcd.print("SIM CONNECTED");
    Serial.println(); delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();         lcd.print("SIM NOT");
    lcd.setCursor(0, 1); lcd.print("   CONNECTED");
    Serial.println(); delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCPIN;
  }

  clear_gsm(); E_count = 0;
ATCREG:
  Serial.print("AT+CREG?"); Serial.println();;
  irq_gsm = 0;
  Serial.println();
  delay(500);
  read_gsm();

  if (gsm[11] == '1')
  {
    lcd.clear();         lcd.print("SIM REGISTRED");
    delay(1000);
    Serial.println(); delay(500);
  }

  else
  {
    lcd.clear();         lcd.print("SIM REGSTRATION");
    lcd.setCursor(0, 1); lcd.print("     FAILED");
    delay(1000);
    E_count++;
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }

  clear_gsm(); E_count = 0;

ATCNMI:
  Serial.print("AT+CNMI=2,1"); Serial.println();
  irq_gsm = 0; Serial.println();
  delay(500);
  read_gsm();

  if (strstr(gsm, "OK") != NULL)
  {
    lcd.clear();
    lcd.print("SMS SETTINGS OK");
    delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print("SMS SETTINGS");
    lcd.setCursor(0, 1);
    lcd.print("  FAILED");
    delay(1000);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCNMI;
  }

}




char gsm_clear_msg(char num)
{
  char E_count = 0;

  clear_gsm(); E_count = 0;
ATCMGD:
  Serial.print("AT+CMGD="); Serial.print(num, DEC);
  irq_gsm = 0;
  Serial.println();
  delay(500);
  read_gsm();

  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear(); lcd.print(" MSG DELETED");
    Serial.println(); delay(100);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print("MSG DELETE ERROR");
    Serial.println(); delay(300);
    if (E_count > 50)
    {
      E_count = 0;
      return (0);
    }
    goto ATCMGD;
  }
  return (1);
}


void gsm_send_num_mesg(char *num1, char *msg1) ///msg deletinging
{
  read_gsm(); clear_gsm(); irq_gsm = 0;
  lcd.clear();  lcd.print("MSG SENDING...");
  lcd.setCursor(0, 1); lcd.print((String)num1);
  delay(500);
  read_gsm(); clear_gsm(); irq_gsm = 0;

  Serial.print("AT+CMGS=");   Serial.print('"');  Serial.print((String)num1);  Serial.print('"');  Serial.println("");  delay(1000);
  Serial.print((String)msg1); Serial.print("  "); Serial.println();  delay(500);
  // Serial.print(msg2); Serial.print("  "); Serial.println();  delay(500);
  //Serial.print((String)msg3); Serial.print("");
  Serial.print(String(gps_lat, 6)); Serial.print(','); Serial.print(String(gps_lng, 6));
  Serial.println();  delay(500);

  irq_gsm = 0; clear_gsm();
  Serial.write(0x1A); delay(1000);
  while (1)
  {
    if (Serial.available())
      break;
  }
  delay(1000);  delay(1000); delay(1000);
  read_gsm();
  delay(3000);
  lcd.clear();
  if (strstr(gsm, "+CMGS:") != NULL)
  {
    lcd.print(" MSG SENT ");
    lcd.setCursor(0, 1); lcd.print("    Successfully");
    delay(2000);
    lcd.clear();
  }
  else
  {
    lcd.print(" MSG Sending ");
    lcd.setCursor(0, 1); lcd.print("Error");
    //Serial.println(" MSG SENT Error ");
    delay(2000);
    lcd.clear();
  }
  Serial.println(); delay(2000);
}

void read_msg()   ///msg reading from gsm
{
  unsigned char a, b, i, count, numcnt, j;

  while (Serial.available())
  {
    j = !Serial.available();
    delay(100);
  }

  Serial.print("AT+CMGR=1");
  Serial.println();

  count = 0;
  i = 0;
  a = 0;
  numcnt = 0;

  while (count != 3)
  {
    while (!Serial.available());
    b = Serial.read();;

    if ((b == ',') || (a == 1))
    {
      if (numcnt < 15)
      {
        if (numcnt > 4)
        {
          mobilenum[numcnt - 5] = b;
        }
        a = 1;
        numcnt++;
      }
      else
        a = 0;
    }
    if (count == 2)
    {
      msg[i] = b;
      i++;
    }
    j = 0;
    if (b == 10)
      count += 1;
  }
  msg[--i] = '\0';
  msg[--i] = '\0';
  mobilenum[10] = '\0';
  Serial.print("AT+CMGD=1");
  Serial.println() ; delay(500);
}
