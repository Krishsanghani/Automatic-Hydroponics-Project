/*

 This Project is developed by sanghani Krish.
 En no:- 216020311008
 Starting developing at 05/08/2023
 Uplode this code to Arduino mega
 use lcd display to monitor TDS, Temperature, Humidity, Soil moisture

*/

// The code is under developing //

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2          
#define DHTTYPE DHT11 
#define TDS_SENSOR_PIN A0
#define NUM_SAMPLES 10

LiquidCrystal_I2C lcd(0x27, 16, 2);  
DHT dht(DHTPIN, DHTTYPE);
String msgString = "";
unsigned char doneS = 0, msgStart = 0;
float finalTds;

int pin1 = 22;
int pin2 = 23;

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  pinMode(pin1,INPUT_PULLUP);
  pinMode(pin2,INPUT_PULLUP);
}

void loop() {
  relaycontrol();
  temptds();
    if(doneS)
    {
        doneS = 0;
      msgString = "";
    }
    float tdsValue = readTDS();
}


void serialEvent()
{
  if(Serial.available())
  {
    char c = Serial.read();
    if(c == '#') { msgStart = 0; doneS = 1; }
    if(msgStart) msgString += c;
    if(c == '*') { msgStart = 1; msgString = ""; doneS = 0; }
  }
}

void relaycontrol()
{
  if(finalTds < 100){ 
    if (msgString == "G100")
    {Serial.println("MSG=G100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F100")
    {Serial.println("MSG=F100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f100")
    {Serial.println("MSG=f100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B100")
    {Serial.println("MSG=B100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 200){ 
    if (msgString == "G200")
    {Serial.println("MSG=G200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F200")
    {Serial.println("MSG=F200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f200")
    {Serial.println("MSG=f200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B200")
    {Serial.println("MSG=B200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 300){ 
    if (msgString == "G300")
    {Serial.println("MSG=G300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F300")
    {Serial.println("MSG=F300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f300")
    {Serial.println("MSG=f300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B300")
    {Serial.println("MSG=B300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 400){ 
    if (msgString == "G400")
    {Serial.println("MSG=G400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F400")
    {Serial.println("MSG=F400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f400")
    {Serial.println("MSG=f400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B400")
    {Serial.println("MSG=B400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 500){ 
    if (msgString == "G500")
    {Serial.println("MSG=G500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F500")
    {Serial.println("MSG=F500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f500")
    {Serial.println("MSG=f500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B500")
    {Serial.println("MSG=B500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 600){ 
   if (msgString == "G600")
    {Serial.println("MSG=G600"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F600")
    {Serial.println("MSG=F600"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f600")
    {Serial.println("MSG=f600"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B600")
    {Serial.println("MSG=B600"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 700){ 
    if (msgString == "G700")
    {Serial.println("MSG=G700"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F700")
    {Serial.println("MSG=F700"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f700")
    {Serial.println("MSG=f700"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B700")
    {Serial.println("MSG=B700"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 800){ 
    if (msgString == "G800")
    {Serial.println("MSG=G800"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F800")
    {Serial.println("MSG=F800"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f800")
    {Serial.println("MSG=f800"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B800")
    {Serial.println("MSG=B800"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }        
  if(finalTds < 900){ 
    if (msgString == "G900")
    {Serial.println("MSG=G900"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F900")
    {Serial.println("MSG=F900"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f900")
    {Serial.println("MSG=f900"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B900")
    {Serial.println("MSG=B900"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }             
  if(finalTds < 1000){ 
    if (msgString == "G1000")
    {Serial.println("MSG=G1000"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1000")
    {Serial.println("MSG=F1000"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1000")
    {Serial.println("MSG=f1000"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1000")
    {Serial.println("MSG=B1000"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }             
  if(finalTds < 1100){ 
    if (msgString == "G1100")
    {Serial.println("MSG=G1100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1100")
    {Serial.println("MSG=F1100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1100")
    {Serial.println("MSG=f1100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1100")
    {Serial.println("MSG=B1100"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }             
  if(finalTds < 1200){ 
  if (msgString == "G1200")
    {Serial.println("MSG=G1200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1200")
    {Serial.println("MSG=F1200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1200")
    {Serial.println("MSG=f1200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1200")
    {Serial.println("MSG=B1200"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }                            
  if(finalTds < 1300){ 
    if (msgString == "G1300")
    {Serial.println("MSG=G1300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1300")
    {Serial.println("MSG=F1300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1300")
    {Serial.println("MSG=f1300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1300")
    {Serial.println("MSG=B1300"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }             
  if(finalTds < 1400){ 
    if (msgString == "G1400")
    {Serial.println("MSG=G1400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1400")
    {Serial.println("MSG=F1400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1400")
    {Serial.println("MSG=f1400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1400")
    {Serial.println("MSG=B1400"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }                  
  if(finalTds < 1500){ 
    if (msgString == "G1500")
    {Serial.println("MSG=G1500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "F1500")
    {Serial.println("MSG=F1500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "f1500")
    {Serial.println("MSG=f1500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    if (msgString == "B1500")
    {Serial.println("MSG=B1500"); Serial.println("relay1 on"); delay(5000); Serial.println("relay1 off"); delay(100); Serial.println("relay2 on"); delay(8000); Serial.println("relay2 off"); delay(100);doneS = 1;}
    }                                                                                                                                                                                             
}

float readTDS() {
  float tdsSum = 0.0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
    int sensorValue = analogRead(TDS_SENSOR_PIN);
    float tdsValue = sensorValue * (5.0 / 1023.0) * (1000.0 / 3.0); 
    tdsSum += tdsValue; 
  }
  float averageTDS = tdsSum / NUM_SAMPLES;
  finalTds = averageTDS;
    return averageTDS;
   // Serial.println(finalTds);
}

void temptds()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  lcd.clear();
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
 // Serial.println(temperature);
  delay(560);
  lcd.clear();
  lcd.print("WATER TDS: ");
  lcd.print(finalTds);
  delay(560);
}


  