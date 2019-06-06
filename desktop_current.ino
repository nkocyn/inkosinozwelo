#include <Adafruit_Fingerprint.h>
#include <Ethernet.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <HCSR04.h>
#include <LiquidCrystal.h>
#include <FlowMeter.h>

int  c=0;

FlowMeter Meter = FlowMeter(2);
const unsigned long period = 5;

void MeterISR() 
{
    Meter.count();
}

int level;
int newlevel;
int oldlevel;

//eeprom
//drivers eeprom balance addresses
int address1 = 0;
int address2 = 1; 
int address3 = 2; 
int address4 = 3; 
int address5 = 4;
int address6 = 5; 
int address7 = 6; 


//drivers eeprom inputs
int f1address = 7;
int f2address = 8;
int f3address = 9;
int f4address = 10;
int f5address = 11;
int f6address = 12;
int f7address = 13;



int remainingaddress1 = 14;
int remainingaddress2 = 15;
int remainingaddress3 = 16;
int remainingaddress4 = 17;
int remainingaddress5 = 18;
int remainingaddress6 = 19;
int remainingaddress7 = 20;
int remainingaddress8 = 21;



//level eeprom addresses
int volumeFilled;
int volumeofTank;
int ultraddress = 22;
int oldaddress=   23;
int volumeFilledaddress =24;

int milleageaddress = 25;
int currentremainingaddress1 =26;
int currentAremainingaddress1=27;
int currentAAremainingaddress1=28;
int transactionaddress=29;
long milleage;
int f1;
int f2;
int f3;
int f4;
int f5;
int f6;
int f7;


//drivers balance variables
double total1;
double first1;
double volume;




int remaining;
int remain;
int transaction;
int count = 0;


const int debounceTime = 20; 
const int numRows = 4; 
const int numCols = 4;



const char keymap[numRows][numCols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const int rowPins[numRows] = { 14, 15, 16, 17 }; 
const int colPins[numCols] = { 18, 19, 20, 21 };
char key;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 10, 179 };
byte server[] = { 192, 168, 10, 226 }; // Api

String result="";
EthernetClient client;


int val;
int percent; 
const int potPin = 15;
int pumpPin=7;
const int rs = 35, en = 36, d4 = 37, d5 = 38, d6 = 39, d7 = 40;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.

SoftwareSerial mySerial(11, 10);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
 Ethernet.begin(mac, ip);
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(1/100);
  Serial.println("\n\nScann finger ");
  finger.begin(57600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scann Finger");
  if (finger.verifyPassword()) 
  {
    ;
  }
  
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(":( no fingerprint sensor");

    
    while (1) 
    { 
      
      delay(1); 
    
    }
    }
finger.getTemplateCount();


for (int row = 0; row < numRows; row++)
{
pinMode(rowPins[row],INPUT); 
digitalWrite(rowPins[row],HIGH); 
}
for (int column = 0; column < numCols; column++)
{
pinMode(colPins[column],OUTPUT); // Set column pins as outputs for writing
digitalWrite(colPins[column],HIGH); // Make all columns inactive
}

attachInterrupt(INT0, MeterISR, RISING);
Meter.reset();
transaction = EEPROM.read(transactionaddress);
 int ff =  EEPROM.read(remainingaddress1);

if(ff!=0){
if(transaction != count){
client.connect(server, 80);
      if (client.connect(server, 80)) 
            {           
  
            int d1=1;
            int mj = EEPROM.read(f1address);
            int ks = EEPROM.read(currentremainingaddress1);
            int nn = EEPROM.read(currentAremainingaddress1);
            int aa =  EEPROM.read(remainingaddress1);
             int mil = EEPROM.read(milleageaddress);
            int gg = aa - nn;
            int hg = mj-ks;
           
            client.print("GET /iot/iotserver.php?"); 
            client.print("drivername="); 
            client.print(d1);
            client.print("&powerfailleft="); 
            client.print(aa);
            client.print("&milleaGe="); 
            client.print(mil);
            client.print("&amountreceivedb4fail="); 
            client.print(hg);
            client.print("&amountreceivedb4failinA="); 
            client.print(gg);
            client.print("&submit=");
            client.println("HTTP/1.1");
            client.println("Host: 192.168.10.226");  
            client.println("User-Agent: Arduino/1.0");
            client.println();  
            hg =0;
            int v =0;
            
            EEPROM.write(milleageaddress, v);
            EEPROM.write(currentremainingaddress1, v);
            EEPROM.write(currentAremainingaddress1, v);
            EEPROM.write(remainingaddress1, v);
            EEPROM.write(f1address, v);
            
            EEPROM.write(currentremainingaddress1, 0);
            EEPROM.write(currentAremainingaddress1, 0);
            EEPROM.write(remainingaddress1, 0);
            EEPROM.write(milleageaddress, 0);
            EEPROM.write(f1address, 0);
 
}
}

}
}
char getKey()
{
char key = 0; // 0 indicates no key pressed
for(int column = 0; column < numCols; column++)
{
digitalWrite(colPins[column],LOW); // Activate the current column.
for(int row = 0; row < numRows; row++) // Scan all rows for a key press.
{
if(digitalRead(rowPins[row]) == LOW) // Is a key pressed?
{
delay(debounceTime); // debounce
while(digitalRead(rowPins[row]) == LOW)
; // wait for key to be released
key = keymap[row][column]; // Remember which key was pressed.
}
}
digitalWrite(colPins[column],HIGH); // De-activate the current column.
}
return key; // returns the key pressed or 0 if none
}











void loop()                     // run over and over again
{

  getFingerprintIDez();
  delay(1);            //don't ned to run this at full speed.

switch(finger.fingerID)
{
  
 case 1:
  inputKeys1();
 break;

}

}









uint8_t getFingerprintID() 
{
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
}

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Driver");
  Serial.println(finger.fingerID);
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
 {
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("unrecognisable!");
  return -1;
 
}




 else{

  // found a match!
  Serial.print("Found ID"); 
  Serial.println(finger.fingerID); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome Driver");
  lcd.print(finger.fingerID);
  
  lcd.setCursor(0,1);
  lcd.print("input mileage prs* ");
   return finger.fingerID; 
}
}





void inputKeys1()
{
char key = getKey();
if( key != 0)// if the character is not 0 then it's a valid key press
{
switch(key)
{
case '0'...'9':
first1 = first1*10+(key - '0');
Serial.println(first1);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(first1);
lcd.setCursor(0,1);
break;

if(key='C')
{
case 'C':
first1 = first1/10;
Serial.println(first1);
lcd.clear();
lcd.setCursor(0,0);
lcd.print(first1);
break;
}

//it stores milleage value into milleage
if(key='*')
{
case '*':
lcd.clear();
int milleage=first1;
Serial.println(milleage);
lcd.setCursor(0,0);
lcd.print("mileage=");
lcd.print(milleage);
lcd.print("km");

lcd.setCursor(0,1);
lcd.print("entr amnt prs #");
break;
}


//it clears the milleage value
if(key='D')
{
case 'D':
first1 = 0;
Serial.println(first1);
lcd.print(first1);
delay(1);
lcd.clear();
lcd.setCursor(0,0);
break;
}


if(key='A')
{
case 'A':

lcd.clear();
lcd.setCursor(0,0);
double l = EEPROM.read(remainingaddress1);//initially = zero
double start1 =EEPROM.read(remainingaddress1);
double start2 =EEPROM.read(remainingaddress1);

int total1=EEPROM.read(address1);
Serial.print("Amount:");
lcd.print("Amount:");
total1=total1-l ;
if(total1>l )
{
do
{
for(l=start1;l > 0;l--)
{
  
EEPROM.write(currentAremainingaddress1,l);
int ss= EEPROM.read(currentAremainingaddress1);
EEPROM.write(remainingaddress1, start2-ss);

delay(period);
Meter.tick(period);

    // output some measurement result
    double rate = Meter.getCurrentFlowrate()/10;
     volume =Meter.getTotalVolume()/10;  

Serial.println("Currently " + String(rate) + " l/min, " + String(volume)+ " Litrs total.");
  
digitalWrite(pumpPin, HIGH);   
transaction = 1;
EEPROM.write(transactionaddress, transaction);                    
EEPROM.write(address1, total1);
Serial.println("Name:Driver1");
Serial.print("left: ");
Serial.println(l);


lcd.clear();
lcd.setCursor(0,0);
lcd.print(String(volume)+ "Ltrs.");
digitalWrite(pumpPin, HIGH);    
transaction=1;
EEPROM.write(transactionaddress, transaction);
EEPROM.write(address1, total1);


lcd.print(String(volume)+ "Litres.");            
lcd.setCursor(0,0);
lcd.print("Name:Driver1");
lcd.setCursor(0,1);
lcd.print("left: ");
lcd.print(l);
}

}while(volume<start1);






digitalWrite(pumpPin, LOW);
int values =EEPROM.read(oldaddress);
int balance=values-start1;
EEPROM.write(oldaddress, balance);
Serial.println("Name:Driver1");
Serial.print("received: ");
Serial.print(start1);
Serial.print("Ltrs.");


lcd.clear();
lcd.setCursor(0,0);
lcd.print("Name:Driver1");
lcd.setCursor(0,1);
lcd.print("received: ");
lcd.print(start1);
lcd.print("Ltrs.");



client.connect(server, 80);
      if (client.connect(server, 80)) 
            {           
  EEPROM.write(currentAAremainingaddress1,start1);
            int d1=1;
            int vv = EEPROM.read(remainingaddress1);
            int k1 = EEPROM.read(oldaddress);
            int sv1 = EEPROM.read(currentAAremainingaddress1);
            int svv1 = sv1-vv;
            int mil =EEPROM.read(milleageaddress);
            client.print("GET /iot/iotserver.php?"); 
            client.print("drivername="); 
            client.print(d1);
            client.print("&amountleft="); 
            client.print(k1);
            client.print("&milleaGe="); 
            client.print(mil);
            client.print("&powerfailamnt="); 
            client.print(svv1);
            client.print("&powerfailleft="); 
            client.print(vv);
            client.print("&submit=");
            client.println("HTTP/1.1");
            client.println("Host: 192.168.10.226");  
            client.println("User-Agent: Arduino/1.0");
            client.println();  
            int v=0;
            EEPROM.write(milleageaddress,v);
            
 }


else 
 {
  ;
  }
//Reading data
unsigned long  start = millis();
int     timeout = 2000;
    
    while (millis() - start < timeout){ 
      while (client.available()) {
char c = client.read();
        result += c;
      }  
    }
  if (!client.connected())
  {
    Serial.println();
    client.flush();
    client.stop();
  }
Serial.println(result);

}
else
{
Serial.println("InsufficientBalance");
digitalWrite(pumpPin, LOW);    // turn the LED off by making the voltage LOW
}
break;
}// of if(key A)

if(key='B')
{
case 'B':
lcd.clear();
lcd.setCursor(0,0);
double rem =EEPROM.read(remainingaddress1);
Serial.print("Amount:");
Serial.print(rem);

rem =EEPROM.read(remainingaddress1);
lcd.print("Amnt:");
lcd.setCursor(6,0);
lcd.print(rem);
lcd.print("ltrs");
delay(8000);
break;
}


case '#':


double volume;
int c = EEPROM.read(remainingaddress1); 
int total1=EEPROM.read(address1);
Serial.print("Amount:");
total1=total1-first1;
int remain=first1;

if(total1>first1)
{
do
{

    delay(period);

    // process the (possibly) counted ticks
    Meter.tick(period);

    // output some measurement result
    double rate = Meter.getCurrentFlowrate()/10;
     volume =Meter.getTotalVolume()/10;
  
remain--;
EEPROM.write(f1address, first1);
EEPROM.write(currentremainingaddress1, remain);
int b=EEPROM.read(currentremainingaddress1);
EEPROM.write(remainingaddress1, c+b);

  Serial.println("Currently " + String(rate) + " l/min, " + String(volume)+ " Litrs total.");



lcd.clear();
lcd.setCursor(0,0);
lcd.print(String(volume)+ "Ltrs.");
digitalWrite(pumpPin, HIGH);    
 transaction=1;
 EEPROM.write(transactionaddress, transaction);
EEPROM.write(address1, total1);
}while(volume<first1);

digitalWrite(pumpPin, LOW);
Serial.println("Name:Driver1");
Serial.print("received: ");
Serial.print(first1);
Serial.print("Ltrs.");

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Name:Driver1");
lcd.setCursor(0,1);
lcd.print("received: ");
lcd.print(first1);
lcd.print("Ltrs.");


int values=EEPROM.read(oldaddress);
int balance=values-first1;
EEPROM.write(oldaddress, balance);
Serial.println(  balance);


newlevel = distanceSensor.measureDistanceCm();
volumeofTank=((555)*(893-newlevel)/(100));

EEPROM.write(ultraddress, volumeofTank);

int ccc = EEPROM.read(ultraddress);
Serial.println(ccc);



client.connect(server, 80);
      if (client.connect(server, 80)) 
            {           
  
            int d1=1;
            int j1 = EEPROM.read(f1address);
            int k1 = EEPROM.read(oldaddress);
            int mil =EEPROM.read(milleageaddress);
            client.print("GET /iot/iotserver.php?"); 
            client.print("drivername="); 
            client.print(d1);
            client.print("&amountreceived="); 
            client.print(j1);
            client.print("&amountleft="); 
            client.print(k1);
            client.print("&milleaGe="); 
            client.print(mil);
            client.print("&submit=");
            client.println("HTTP/1.1");
            client.println("Host: 192.168.10.226");  
            client.println("User-Agent: Arduino/1.0");
            client.println();  
            int v=0;
            EEPROM.write(milleageaddress,v);
            
 }

else 
 {
  ;
  }
//Reading data
    unsigned long start = millis();
    int timeout = 2000;
    
    while (millis() - start < timeout){ 
      while (client.available()) {
        char c = client.read();
        result += c;
      }  
    }
  if (!client.connected())
  {
    Serial.println();
    client.flush();
    client.stop();
  }
Serial.println(result);


transaction = count = 0;
 EEPROM.write(transactionaddress, transaction);

}

else
{
Serial.println("InsufficientBalance");
lcd.clear();
lcd.setCursor(0,0);
lcd.print("InsufficientBalance:(");
digitalWrite(pumpPin, LOW);    // turn the LED off by making the voltage LOW
}


break; // # break

}
}
}
