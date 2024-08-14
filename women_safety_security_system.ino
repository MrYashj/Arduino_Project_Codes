#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
LiquidCrystal lcd(2,3,4,5,6,7 ); //RS,EN,D4-D7

int button = 53;
int buz = 13;


static const int RXPin = 8, TXPin =9;
static const uint32_t GPSBaud = 9600;
char st = 0;
float gps_lat, gps_lng;

TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
void gps_read();
void displayInfo();
char z = 0;



char gsm[50], ms, msg[20], irq_gsm = 0, mobilenum[10];

void gsm_init(void);
void gsm_send_num_mesg(char *num1, char *msg1, char *msg3);
void read_msg();
char gsm_clear_msg(char num);
void gpsread();


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
  lcd.begin(16, 2);
  Serial.begin(9600);
  ss.begin(GPSBaud);

  pinMode(button, INPUT_PULLUP);
  pinMode(buz, OUTPUT);
  digitalWrite(buz, HIGH);


  lcd.setCursor(0, 0);   lcd.print("  Women Safty ");
  lcd.setCursor(0, 1);   lcd.print("    System    ");   delay(2000);
  digitalWrite(buz, LOW);

  gsm_init();
  gsm_clear_msg(1);
  gsm_clear_msg(2);


  back:
    lcd.clear();
    lcd.setCursor(0,0);
   lcd.print("send any msg");
   lcd.setCursor(0,1);
   lcd.print("store your num..");
   delay(500);
   while(Serial.available()==0);
     if(Serial.available()>0)
    {
     delay(300);
     Serial.print("AT+CMGR=1"); irq_gsm = 0;  Serial.println();delay(500);
  
     read_gsm();
  
     if(strstr(gsm,"+CMTI:")!=NULL)
     {
      delay(100);
      read_msg();
  
      lcd.setCursor(0,1);lcd.print((String)mobilenum);
      delay(3000);
  
      gsm_send_num_mesg(mobilenum,"Ur number stored successfully","");
      irq_gsm=0;clear_gsm();
     }
     else
       goto back;
    }

  while (1)
  {
    gps_read();

    if (st == 1)
    {
      st = 0;
      break;
    }
  }
  Serial.println("Values got");
  delay(2000);
}


void loop()
{
xyz:
  z = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  women safty ");
  lcd.setCursor(0, 1);
  lcd.print("    system    ");
  delay(500);
  if (digitalRead(button) == LOW)
  {
    digitalWrite(buz, HIGH);
    lcd.clear();
    lcd.print("button pressed");
    delay(1000);
    while (1)
    {
      gps_read();

      if (st == 1)
      {
        st = 0;
        break;
      }
    }
    digitalWrite(buz, LOW);
    gsm_send_num_mesg(mobilenum, "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
    delay(1000);
    gsm_send_num_mesg("8328188687", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
    delay(1000);
//    gsm_send_num_mesg("6281087331", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
//    delay(1000);
//    gsm_send_num_mesg("9063345266", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
//    delay(1000);
//    gsm_send_num_mesg("8886269295", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
//    delay(1000);
//    gsm_send_num_mesg("7680000550", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
//    delay(1000);
//    gsm_send_num_mesg("8143465908", "SMS from Women safty device: \nEmergency plz help:", "http://google.co.in/maps/place/");
//    delay(1000);

    irq_gsm = 0; clear_gsm();
    goto xyz;
  }

  if (Serial.available() > 0)
  {
    clear_gsm();
    Serial.print("AT+CMGR=1"); irq_gsm = 0;  Serial.println(); delay(1000);
    irq_gsm = 0;
    clear_gsm();
    read_gsm();

    if (strstr(gsm, "+CMTI:") != NULL)
    {
      lcd.clear();
      lcd.print("Got sms"); delay(1000);
      read_msg();
      mobilenum[10] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(msg);
      lcd.setCursor(0, 1);
      lcd.print(mobilenum);
      delay(1000);
      if (!strcmp(msg, "Track"))
      {
        gsm_send_num_mesg(mobilenum, "Present location:", "http://google.co.in/maps/place/"); delay(2000);
        delay(1000);
        irq_gsm = 0; clear_gsm();
      }
    }
  }

  delay(500);

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
    lcd.clear();       lcd.print("Pls Check Ur");
    lcd.setCursor(0, 1); lcd.print("Connections");
    delay(2000);

  }
}
void displayInfo()
{
  lcd.clear(); lcd.print("GPS READING..");
  delay(2000);

  if (gps.location.isValid())
  {
    gps_lat = gps.location.lat();
    gps_lng = gps.location.lng();
    lcd.clear();
    lcd.print("LAT:"); lcd.print(String(gps_lat, 6));
    lcd.setCursor(0, 1);
    lcd.print("LNG:"); lcd.print(String(gps_lng, 6));

    Serial.print(String(gps_lat, 6));
    Serial.print(F(","));
    Serial.print(String(gps_lng, 6));
    delay(2000);
    st = 1;
  }
  else
  {
    lcd.clear(); lcd.print("INVALID VALUES");
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
  lcd.clear(); lcd.print("ATE0: "); delay(500);
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
    lcd.setCursor(0, 1); lcd.print("     NOT OK");  Serial.println();  delay(1000);
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
    lcd.setCursor(0, 1); lcd.print("   CONNECTED");  Serial.println(); delay(800);
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
    lcd.setCursor(0, 1); lcd.print("     FAILED");  delay(1000);
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
    lcd.print("  FAILED");   delay(1000);
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
    lcd.clear(); lcd.print(" MSG DELETED");  Serial.println(); delay(100);
  }
  else
  {
    E_count++;
    lcd.clear(); lcd.print("MSG DELETE ERROR");  Serial.println(); delay(300);
    if (E_count > 50)
    {
      E_count = 0;
      return (0);
    }
    goto ATCMGD;
  }
  return (1);
}


void gsm_send_num_mesg(char *num1, char *msg1, char *msg3) ///msg deletinging
{
  read_gsm(); clear_gsm(); irq_gsm = 0;
  lcd.clear();  lcd.print("MSG SENDING...");
  lcd.setCursor(0, 1); lcd.print((String)num1); delay(500);
  read_gsm(); clear_gsm(); irq_gsm = 0;

  Serial.print("AT+CMGS=");   Serial.print('"');  Serial.print((String)num1);  Serial.print('"');  Serial.println("");  delay(1000);
  Serial.print((String)msg1); Serial.print("  "); Serial.println();  delay(500);
  // Serial.print(msg2); Serial.print("  "); Serial.println();  delay(500);
  Serial.print((String)msg3); Serial.print("");
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
  }
  else
  {
    lcd.print(" MSG Sending ");
    lcd.setCursor(0, 1); lcd.print("Error");
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
