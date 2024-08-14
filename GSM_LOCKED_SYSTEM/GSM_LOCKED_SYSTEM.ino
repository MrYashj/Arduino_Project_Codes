#include <Keypad.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //RS,EN,D4-D7

const int LDRPin = A1;      // LDR pin connected to analog pin A0
const int ledPin = 24;      // LED pin connected to digital pin 13
const int vibratorPin = 11;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {14,15,16,17}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {18,19,20,21}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int relayPin = 10; // Connect relay to analog pin A0
const String password = "AAAA"; // Set your password here


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
char mobilenum[] = "8328188687";
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

void setup()
{ 
    lcd.setCursor(0,0);
  lcd.print("GSM LOCK SYSTEM");
  delay(3000);
   pinMode(relayPin, OUTPUT); // Initialize relay pin as output
  digitalWrite(relayPin, LOW); // Initially turn off the relay
  lcd.begin(16, 2); // Initialize the LCD
  lcd.print("Enter password:");

    pinMode(vibratorPin, OUTPUT);
   pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);       
 // pinMode(relayPin, OUTPUT); 
  //digitalWrite(relayPin, HIGH); 
 // lcd.setCursor(0,0);
 // lcd.print("GSM LOCK SYSTEM");
  //delay(2000);
  //lcd.clear();
  //lcd.begin(16, 2); // Initialize the LCD
  //lcd.print("Enter password:");
  //Serial.begin(9600);



  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME"); delay(1000);   lcd.clear();

  gsm_init();
  gsm_clear_msg(1);
  gsm_clear_msg(2);
  lcd.clear();
   pinMode(relayPin, OUTPUT); // Initialize relay pin as output
  digitalWrite(relayPin, HIGH); // Initially turn off the relay
  lcd.begin(16, 2); // Initialize the LCD
  lcd.print("Enter password:");

  
   pinMode(ledPin, OUTPUT);
  Serial.println((String)mobilenum);//comment this line if ur not using in built Number

  if (st == 1)
  {
    st = 0;
  }

}
unsigned int i;



void loop()
{  
   if (digitalRead(vibratorPin) == HIGH)
   {
     gsm_send_num_mesg("8328188687", "Glass is breaking at home");
   }


  
  int lightLevel = analogRead(LDRPin);  // Read the analog value from the LDR
  
  // Check if the light level is below a certain threshold (adjust as needed)
  if (lightLevel <500) {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
  } else {
    digitalWrite(ledPin, LOW);   // Turn off the LED
  }
  
  delay(500);  // Add a small delay to reduce sensor reading frequency

  
  char key = keypad.getKey();
  
  if (key) {
    static String enteredPassword = "";
    if (key == 'D') {
      if (enteredPassword == password) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Password OK!");
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Wrong password!");
        delay(2000);
       gsm_send_num_mesg("8328188687", "theft alert from House:");
        
      }
      enteredPassword = ""; // Reset entered password
      delay(1000); // Delay before clearing the LCD
      lcd.clear();
      lcd.print("Enter password:");
    }
    else
    {
      enteredPassword += key; // Add pressed key to entered password
      lcd.setCursor(0, 1);
      lcd.print(enteredPassword); // Display entered password
       
    }
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
    delay(2000);

  }
}



void displayInfo()
{
  Serial.print("GPS READING..");
  delay(2000);

  if (gps.location.isValid())
  {
    gps_lat = gps.location.lat();
    gps_lng = gps.location.lng();

    Serial.print(String(gps_lat, 6));
    Serial.print(F(","));
    Serial.println(String(gps_lng, 6));
    delay(2000);
    st = 1;
  }
  else
  {
    Serial.println("INVALID VALUES");
    st = 0;
    delay(2000);
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
    //Serial.println(" MSG SENT ");
  }
  else
  {
    lcd.print(" MSG Sending ");
    lcd.setCursor(0, 1); lcd.print("Error");
    //Serial.println(" MSG SENT Error ");
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
