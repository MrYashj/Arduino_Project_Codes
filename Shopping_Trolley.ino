#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HCSR04.h>
HCSR04 hc(6, 7);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define add_button A1
#define remove_button A2
#define buz A0
#define button 4
SoftwareSerial gs(8, 9);

String firstTenNumbers = "";
char gsm[50], ms, msg[20], irq_gsm = 0, mobilenum[10];
void gsm_init(void);
void gsm_send_num_mesg(String num1, String msg1, String msg3);
void read_msg();
char gsm_clear_msg(char num);



void clear_gsm()
{
  unsigned char uc;
  for (uc = 0; uc < 50; uc++)
    gsm[uc] = '\0';
}

void read_gsm(void)
{
  while (gs.available())
  {
    gsm[irq_gsm] = gs.read();
    irq_gsm++;
    if (irq_gsm > 50)
      irq_gsm = 50;
  }
}


void setup()
{
  gs.begin(9600);
  Serial.begin(9600);         // Initialize serial communication
  lcd.init();                // Turn on the blacklight and print a message.
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522

  lcd.backlight();            // initialize the lcd
  pinMode(buz, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(add_button, INPUT_PULLUP);
  pinMode(remove_button, INPUT_PULLUP);
  digitalWrite(buz, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Smart Shoping");
  lcd.setCursor(0, 1);
  lcd.print("    Trolly    ");
  delay(2000);
  digitalWrite(buz, LOW);

  gsm_init();
  gsm_clear_msg(1);
  gsm_clear_msg(2);
  delay(2000);
}
int vsms_s = 0;
int amount = 0;
int add_s = 0;
int sub_s = 0;
bool check_theft =  true;
int qty = 0;
void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (qty <= 0)
    qty = 0;
  lcd.print("Qty:" + String(qty));
  lcd.setCursor(0, 1);
  lcd.print("T_amount:" + String(amount));
  delay(1000);
  if (digitalRead(button) == LOW)
  {
    digitalWrite(buz, HIGH);
    lcd.clear();
    lcd.print("  Thank you  ");
    lcd.setCursor(0, 1);
    lcd.print("Welcome Again");
    delay(2000);
    digitalWrite(buz, LOW);
    if(amount>0)
    {
    gsm_send_num_mesg(firstTenNumbers, "Total_Amount:" + String(amount), "\nPay throught this link:\n https://rzp.io/I/zMBMqjEI"); delay(2000);
    irq_gsm = 0;
    }
    qty = 0;
    amount = 0;
    add_s = 0;
    sub_s = 0;
  }
  if (check_theft == true)
  {
    int dis = hc.dist(); delay(60);
    if (dis < 30)
    {
      digitalWrite(buz, HIGH); delay(4000);  digitalWrite(buz, LOW);
    }
    else
    {
      digitalWrite(buz, LOW);
    }
  }

  if (digitalRead(add_button) == LOW)
  {
    add_s = 1;
    sub_s = 0;
  }
  if (digitalRead(remove_button) == LOW)
  {
    add_s = 0;
    sub_s = 1;
  }
  if (add_s == 1 || sub_s == 1)
  {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {

      // Print UID of the card
      Serial.print("UID Tag :");
      String content = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      content.toUpperCase(); // Convert to uppercase for consistent comparison
      Serial.println(content);

      // Compare content with "53763FDD" (without spaces)
      //    if (content == "53763FDD") {
      //      Serial.println("Content matches!");
      //    } else {
      //      Serial.println("Content does not match!");
      //    }

      if (content == "53763FDD")
      {
        if (add_s == 1)
        {
          qty++;
          amount = amount + 100;
          Serial.println("Oil Added");
          lcd.clear();
          lcd.print("Oil Added");
        }
        if (sub_s == 1)
        {
          qty--;
          amount = amount - 100;
          if (amount <= 0)
            amount = 0;
          Serial.println("Oil Removed");
          lcd.clear();
          lcd.print("Oil Removed");
        }
        lcd.setCursor(0, 1);
        lcd.print("T_amount:" + String(amount)); delay(3000);
      }

      if (content == "C3367BC5")
      {
        if (add_s == 1)
        {
          qty++;
          amount = amount + 50;
          Serial.println("Sugar Added");
          lcd.clear();
          lcd.print("Sugar Added");
        }
        if (sub_s == 1)
        {
          qty--;
          amount = amount - 50;
          if (amount <= 0)
            amount = 0;
          Serial.println("Sugar Removed");
          lcd.clear();
          lcd.print("Sugar Removed");
        }
        lcd.setCursor(0, 1);
        lcd.print("T_amount:" + String(amount)); delay(3000);
      }

      if (content == "F36112FC")
      {
        if (add_s == 1)
        {
          qty++;
          amount = amount + 50;
          Serial.println("Rice Added");
          lcd.clear();
          lcd.print("Rice Added");
        }
        if (sub_s == 1)
        {
          qty--;
          amount = amount - 50;
          if (amount <= 0)
            amount = 0;
          Serial.println("Rice Removed");
          lcd.clear();
          lcd.print("Rice Removed");
        }
        lcd.setCursor(0, 1);
        lcd.print("T_amount:" + String(amount)); delay(3000);
      }

      if (content == "1365E8FA")
      {
        if (add_s == 1)
        {
          qty++;
          amount = amount + 10;
          Serial.println("Salt Added");
          lcd.clear();
          lcd.print("Salt Added");
        }
        if (sub_s == 1)
        {
          qty--;
          amount = amount - 10;
          if (amount <= 0)
            amount = 0;
          Serial.println("Salt Removed");
          lcd.clear();
          lcd.print("Salt Removed");
        }
        lcd.setCursor(0, 1);
        lcd.print("T_amount:" + String(amount)); delay(3000);
      }
    }
  }

  /*************************************************************/
  if (gs.available() > 0)
  {
    delay(300);
    gs.print("AT+CMGR=1");
    irq_gsm = 0;
    gs.println();
    delay(500);
    read_gsm();
    if (strstr(gsm, "+CMTI:") != NULL)
    {
      delay(100);
      read_msg();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print((String)msg);
      lcd.setCursor(0, 1);
      lcd.print((String)mobilenum);
      delay(3000);
    }
  }
}




void gsm_init(void)///gsm working or not checking
{
  unsigned int ui_t1;
  char E_count = 0;

gsminit:
  lcd.clear();
  lcd.print("GSM INIT...");
  gs.print("ATE0");
  gs.println();
  delay(500);
  irq_gsm = 0;

AT:
  gs.println();
  delay(100);
  gs.print("AT");
  gs.println();
  lcd.clear();
  lcd.print("AT: ");
  delay(500);
  irq_gsm = 0;
  read_gsm();

  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print(" AT FAILED");
    delay(1000);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto AT;
  }

  clear_gsm();
  E_count = 0;

  gs.print("ATE0");
  irq_gsm = 0;
  gs.println();
  gs.println();
  delay(500);

  clear_gsm();
  E_count = 0;
ATE0:
  gs.print("ATE0");
  irq_gsm = 0;
  gs.println();
  gs.println();
  lcd.clear();
  lcd.print("ATE0: ");
  delay(500);
  read_gsm();
  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print("ATE0 FAILED");
    delay(500);
    if (E_count > 9)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATE0;
  }

  clear_gsm();
  E_count = 0;

ATCMGF:
  gs.print("AT+CMGF=1");
  gs.println();
  lcd.clear();
  lcd.print("TEXT MODE: ");
  irq_gsm = 0;
  gs.println();
  delay(500);
  read_gsm();
  if (strstr(gsm, "OK") != NULL)
  {
    lcd.print("OK");
    delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print("TEXT MODE");
    lcd.setCursor(0, 1);
    lcd.print("     NOT OK");
    gs.println();
    delay(1000);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCMGF;
  }

  clear_gsm();
  E_count = 0;

ATCPIN:
  gs.print("AT+CPIN?");
  gs.println();
  irq_gsm = 0;
  gs.println();
  delay(500);
  read_gsm();

  if (gsm[2] == '+' && gsm[3] == 'C' && gsm[4] == 'P' && gsm[5] == 'I' && gsm[6] == 'N' && gsm[7] == ':' && gsm[8] == ' ' && gsm[9] == 'R')
  {
    lcd.clear();
    lcd.print(" SIM CONNECTED");
    delay(1000);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print("SIM NOT");
    lcd.setCursor(0, 1);
    lcd.print("   CONNECTED");
    delay(800);
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCPIN;
  }

  clear_gsm();
  E_count = 0;
ATCREG:
  gs.print("AT+CREG?");
  gs.println();
  irq_gsm = 0;
  gs.println();
  delay(500);
  read_gsm();

  if (gsm[11] == '1')
  {
    lcd.clear();
    lcd.print(" SIM REGISTRED");
    delay(1000);
    gs.println();
    delay(500);
  }

  else
  {
    lcd.clear();
    lcd.print("SIM REGISTRATION");
    lcd.setCursor(0, 1);
    lcd.print("     FAILED");
    delay(1000);
    E_count++;
    if (E_count > 20)
    {
      E_count = 0;
      goto gsminit;
    }
    goto ATCREG;
  }

  clear_gsm();
  E_count = 0;

ATCNMI:
  gs.print("AT+CNMI=2,1");
  gs.println();
  irq_gsm = 0;
  gs.println();
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

  clear_gsm();
  E_count = 0;
ATCMGD:
  gs.print("AT+CMGD=");
  gs.print(num, DEC);
  irq_gsm = 0;
  gs.println();
  delay(500);
  read_gsm();

  if (gsm[2] == 'O' && gsm[3] == 'K')
  {
    lcd.clear();
    lcd.print(" MSG DELETED");
    delay(100);
  }
  else
  {
    E_count++;
    lcd.clear();
    lcd.print("MSG DELETE ERROR");
    delay(300);
    if (E_count > 50)
    {
      E_count = 0;
      return (0);
    }
    goto ATCMGD;
  }
  return (1);
}

void gsm_send_num_mesg(String num1, String msg1, String msg3) ///msg deletinging
{
  read_gsm();
  clear_gsm();
  irq_gsm = 0;
  lcd.clear();
  lcd.print("MSG SENDING...");
  lcd.setCursor(0, 1);
  lcd.print(num1);
  delay(500);
  read_gsm();
  clear_gsm();
  irq_gsm = 0;

  gs.print("AT+CMGS=");
  gs.print('"');
  gs.print((String)num1);
  gs.print('"');
  gs.println("");
  delay(1000);
  gs.print((String)msg1);
  gs.print("  ");
  gs.println();
  delay(500);
  gs.print((String)msg3);
  gs.print("");
  gs.println();
  delay(500);

  irq_gsm = 0;
  clear_gsm();
  gs.write(0x1A);
  delay(1000);
  while (1)
  {
    if (gs.available())
      break;
  }
  delay(1000);
  delay(1000);
  delay(1000);
  read_gsm();
  delay(3000);
  lcd.clear();
  if (strstr(gsm, "+CMGS:") != NULL)
  {
    lcd.print(" MSG SENT ");
    lcd.setCursor(0, 1);
    lcd.print("    Successfully");
  }
  else
  {
    lcd.print(" MSG Sending ");
    lcd.setCursor(0, 1);
    lcd.print("Error");
  }
  gs.println();
  delay(2000);
}

void read_msg() ///msg reading from gsm
{
  unsigned char a, b, i, count, numcnt, j;

  while (gs.available())
  {
    j = !gs.available();
    delay(100);
    Serial.print(j);
  }

  gs.print("AT+CMGR=1");
  gs.println();

  count = 0;
  i = 0;
  a = 0;
  numcnt = 0;

  while (count != 3)
  {
    while (!gs.available());
    b = gs.read();
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
  gs.print("AT+CMGD=1");
  gs.println();
  delay(500);
  firstTenNumbers = "";
  for (int i = 0; i < 10; i++)
  {
    firstTenNumbers += mobilenum[i]; delay(10);
  }

}
