#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MsTimer2.h>
#include <TimerOne.h>

#define G1_TDS_TIME 4
#define G2_TDS_TIME 5
#define G3_TDS_TIME 6
#define G4_TDS_TIME 7

#define F1_TDS_TIME 8
#define F2_TDS_TIME 9
#define F3_TDS_TIME 10
#define F4_TDS_TIME 11

#define f1_TDS_TIME 12
#define f2_TDS_TIME 13
#define f3_TDS_TIME 14
#define f4_TDS_TIME 15

#define B1_TDS_TIME 16
#define B2_TDS_TIME 17
#define B3_TDS_TIME 18
#define B4_TDS_TIME 19

char serialData[] = "*T000H000M000A0000B0000C0000#";
//T - Temperature 000   H - Humidity 000  M - Soil Moisture A - TDS1  B - TDS2  C - TDS3

#define T_Address 2
#define H_Address 6
#define M_Address 10
#define T1_Address 14
#define T2_Address 19
#define T3_Address 24

#define DHTPIN 2          
#define DHTTYPE DHT11 
#define TDS_SENSOR_PIN0 0
#define TDS_SENSOR_PIN1 1
#define TDS_SENSOR_PIN2 2
#define NUM_SAMPLES 10

LiquidCrystal_I2C lcd(0x27, 16, 2);  
DHT dht(DHTPIN, DHTTYPE);
String msgString = "";
unsigned char doneS = 0, msgStart = 0;
float finalTDS = 0.0;
float humidity = 0.0, temp = 0.0;
unsigned char index = 0;
volatile unsigned int serialTimeout = 0;
unsigned char r1ONTime = 0, r2ONTime = 0, r3ONTime = 0, r4ONTime = 0, r5ONTime = 0, r6ONTime = 0, r7ONTime = 0;
bool operateTimer = 0;

int pin2 = 23;

volatile unsigned int timeCount = 0;
volatile bool Germination = 0, Flowering = 0, fruiting = 0, Boosting = 0, waitProcess = 0;
unsigned int userTDS = 0;

unsigned int timingS[8] = {0,0,0,0,0,0,0,0};
unsigned int maxTiming = 0, gIndex = 0;
int pin1 = 5;

void setup() {
  pinMode(30,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(28,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(30,LOW);
  digitalWrite(24,LOW);
  digitalWrite(26,LOW);
  digitalWrite(28,LOW);
  pinMode(pin1,INPUT_PULLUP);
  pinMode(pin2,INPUT_PULLUP);
  
  Serial.begin(115200);
  while(!Serial);
  lcd.init();
  lcd.backlight();
  dht.begin();
  
  MsTimer2::set(100, operateProcess);
  MsTimer2::start();
}

void operateProcess()
{
  if(operateTimer)
  {
      timeCount--;
      if(timeCount<=0)
      {
        waitProcess = 1;
        digitalWrite(13,LOW);
      }
  }

  /*
  serialTimeout++;
  if(serialTimeout>=4) 
  {
    Serial.println(serialData);
    serialTimeout = 0;
  }
  */
  
}



unsigned int getMaxValue(unsigned int arr[], int size) {
  unsigned int maxVal = arr[0];  // Initialize maxVal with the first element

  for (int i = 1; i < size; i++) {
    if (arr[i] > maxVal) {
      maxVal = arr[i];  // Update maxVal if the current element is greater
    }
  }

  return maxVal;
}

void loop() 
{

    readTDS(index); index++; if(index>2) index = 0;
    Temp_Humidity();
    
  if(doneS)
  {
      relaycontrol();
      doneS = 0;
      msgString = "";
  }

  if(operateTimer)
  {
      if(Germination == 1)
      {
        for(gIndex = 0; gIndex<=7; gIndex++) timingS[gIndex] = 0;
        timingS[1] = G1_TDS_TIME * (userTDS);
        timingS[5] = G2_TDS_TIME * (userTDS);
        timingS[6] = G3_TDS_TIME * (userTDS);
        timingS[7] = G4_TDS_TIME * (userTDS);
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
          Serial.print("G"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
        }
        maxTiming = getMaxValue(timingS, 8);
        Serial.print("Max Time : "); Serial.println(maxTiming);
         
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
            waitProcess = 0;
            switch(gIndex){
             case 1:
                   digitalWrite(24,timingS[1]);
                   
                   break;
             case 5:
                  digitalWrite(24,LOW);
                  digitalWrite(26,timingS[5]);
                  break;  
             case 6:
                  digitalWrite(26,LOW);
                  digitalWrite(28,timingS[6]);
                  break;  
             case 7:
                  digitalWrite(28,LOW);
                  digitalWrite(30,timingS[7]);
                  break;
                                  
            }
            timeCount = timingS[gIndex] / 10;
            if(timeCount>0) 
            {
              Serial.print("Relay ON: "); Serial.println(gIndex);
              while(waitProcess==0) 
              { 
                  readTDS(index); index++; if(index>2) index = 0;
                  Temp_Humidity();
              }
            }
        }
      }

      if(Flowering == 1)
      {
        for(gIndex = 0; gIndex<=7; gIndex++) timingS[gIndex] = 0;
        timingS[2] = F1_TDS_TIME * (userTDS);
        timingS[5] = F2_TDS_TIME * (userTDS);
        timingS[6] = F3_TDS_TIME * (userTDS);
        timingS[7] = F4_TDS_TIME * (userTDS);
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
          Serial.print("F"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
        }
        maxTiming = getMaxValue(timingS, 8);
        Serial.print("Max Time : "); Serial.println(maxTiming);
        
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
            waitProcess = 0; 
            digitalWrite(13,HIGH);
            
            timeCount = timingS[gIndex] / 10;
            if(timeCount>0) 
            {
              Serial.print("Relay ON: "); Serial.println(gIndex);
              while(waitProcess==0) 
              { 
                  readTDS(index); index++; if(index>2) index = 0;
                  Temp_Humidity();
              }
            }
        }
      }

      if(fruiting == 1)
      {
        for(gIndex = 0; gIndex<=7; gIndex++) timingS[gIndex] = 0;
        timingS[3] = f1_TDS_TIME * (userTDS);
        timingS[5] = f2_TDS_TIME * (userTDS);
        timingS[6] = f3_TDS_TIME * (userTDS);
        timingS[7] = f4_TDS_TIME * (userTDS);
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
          Serial.print("f"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
        }
        maxTiming = getMaxValue(timingS, 8);
        Serial.print("Max Time : "); Serial.println(maxTiming);
        
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
            waitProcess = 0; digitalWrite(13,HIGH);
            
            timeCount = timingS[gIndex] / 10;
            if(timeCount>0) 
            {
              Serial.print("Relay ON: "); Serial.println(gIndex);
              while(waitProcess==0) 
              { 
                  readTDS(index); index++; if(index>2) index = 0;
                  Temp_Humidity();
              }
            }
        }
      }

      if(Boosting == 1)
      {
        for(gIndex = 0; gIndex<=7; gIndex++) timingS[gIndex] = 0;
        timingS[4] = B1_TDS_TIME * (userTDS);
        timingS[5] = B2_TDS_TIME * (userTDS);
        timingS[6] = B3_TDS_TIME * (userTDS);
        timingS[7] = B4_TDS_TIME * (userTDS);
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
          Serial.print("B"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
        }
        maxTiming = getMaxValue(timingS, 8);
        Serial.print("Max Time : "); Serial.println(maxTiming);
        
        for(gIndex = 0; gIndex<=7; gIndex++)
        {
            waitProcess = 0; digitalWrite(13,HIGH);
            
            timeCount = timingS[gIndex] / 10;
            if(timeCount>0) 
            {
              Serial.print("Relay ON: "); Serial.println(gIndex);
              while(waitProcess==0) 
              { 
                  readTDS(index); index++; if(index>2) index = 0;
                  Temp_Humidity();
                
              }
            }
        }
      }
      Serial.print("Process Completed!");
       digitalWrite(30,LOW); 
      operateTimer = 0;
  }
//Display_System();
}

/*void Display_System()
{
   lcd.clear();
   lcd.print("Humidity: ");
   lcd.print(humidity);
   lcd.print("%");
   lcd.setCursor(0, 1);
   lcd.print("Temp: ");
   lcd.print(temp);
   lcd.print("C");
   delay(360);
}*/


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
  if(finalTDS < 100)
  {
    userTDS = (msgString[1]- 48) * 1000 + (msgString[2] - 48) * 100 + (msgString[3] - 48) * 10 + (msgString[4] - 48);
    if(msgString[0] == 'G') 
    {
      Serial.println("MSG = "+msgString);
      operateTimer = 1; Germination = 1; Flowering = 0; fruiting = 0; Boosting = 0;
    }
    if(msgString[0] == 'F') 
    {
      Serial.println("MSG = "+msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 1; fruiting = 0; Boosting = 0;
    }
    if(msgString[0] == 'f') 
    {
      Serial.println("MSG = "+msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 0; fruiting = 1; Boosting = 0;
    }
    if(msgString[0] == 'B') 
    {
      Serial.println("MSG = "+msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 0; fruiting = 0; Boosting = 1;
    }
  }
}

void readTDS(unsigned char channel) {
  float tdsSum = 0.0, tdsValue = 0.0;
  unsigned int averageTDS = 0;
  int sensorValue = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
    sensorValue = analogRead(channel);
    tdsValue = sensorValue * (5.0 / 1023.0) * (1000.0 / 3.0); 
    tdsSum += tdsValue; 
  }
  averageTDS = (unsigned int)(tdsSum / NUM_SAMPLES);

  switch(channel)
  {
    case 0: serialData[T1_Address]   = averageTDS/1000%10 + 48;
            serialData[T1_Address+1] = averageTDS/100%10 + 48;
            serialData[T1_Address+2] = averageTDS/10%10 + 48;
            serialData[T1_Address+3] = averageTDS/1%10 + 48;
            break;
    case 1: serialData[T2_Address]   = averageTDS/1000%10 + 48;
            serialData[T2_Address+1] = averageTDS/100%10 + 48;
            serialData[T2_Address+2] = averageTDS/10%10 + 48;
            serialData[T2_Address+3] = averageTDS/1%10 + 48;
            break;
    case 2: serialData[T3_Address]   = averageTDS/1000%10 + 48;
            serialData[T3_Address+1] = averageTDS/100%10 + 48;
            serialData[T3_Address+2] = averageTDS/10%10 + 48;
            serialData[T3_Address+3] = averageTDS/1%10 + 48;
            break;
  }

}

void Temp_Humidity()
{
  unsigned int humidity = 0, temp = 0;
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  serialData[H_Address]    = humidity/100%10 + 48;
  serialData[H_Address+1]  = humidity/10%10 + 48;
  serialData[H_Address+2]  = humidity/1%10 + 48;

  serialData[T_Address]    = temp/100%10 + 48;
  serialData[T_Address+1]  = temp/10%10 + 48;
  serialData[T_Address+2]  = temp/1%10 + 48;
  
    
}


  