#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MsTimer2.h>
#include <TimerOne.h>

#define G1_TDS_TIME 4
#define G2_TDS_TIME 4
#define G3_TDS_TIME 0
#define G4_TDS_TIME 0

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

LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHTPIN, DHTTYPE);
String msgString = "";
float finalTDS = 0.0;
unsigned int humidity = 0, temp = 0;
unsigned char index = 0, doneS = 0, msgStart = 0;
volatile unsigned int serialTimeout = 0;
unsigned char r1ONTime = 0, r2ONTime = 0, r3ONTime = 0, r4ONTime = 0, r5ONTime = 0, r6ONTime = 0, r7ONTime = 0;
bool operateTimer = 0, mixing = 0;
volatile unsigned int timeCount = 0;
volatile bool Germination = 0, Flowering = 0, fruiting = 0, Boosting = 0, waitProcess = 0;
unsigned int userTDS = 0, valve = 0, flag = 0, valvecontrol = 0, averageTDS = 0;
unsigned int tds1 = 0, tds2 = 0, tds3 = 0;
unsigned int timingS[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char maxTiming = 0, gIndex = 0;
unsigned char pump_pins[7] = {23, 25, 27, 29, 31, 33, 35};
unsigned char fan_pins[10] = {37, 39, 41, 43, 45, 47, 49, 51, 53, 24};
unsigned char valve_pins[3] = {24, 26, 28};
char display1[9] = {'A', 'u', 't', 'o', 'm', 'a', 't', 'i', 'c'};
char display2[18] = {'H', 'y', 'd', 'r', 'o', 'p', 'o', 'n', 'i', 'c', 's', ' ', 'S', 'y', 's', 't', 'e', 'm'};
char display3[13] = {'M', 'a', 'd', 'e', ' ', 'B', 'y', ' ', 'K', 'r', 'i', 's', 'h'};
/*char display3[6] = {'s', 'y', 's', 't', 'e', 'm'};*/

void setup() {
  lcd.begin(20, 4);
  lcd.init();
  lcd.init();
  lcd.backlight();

  for (unsigned char index = 0; index <= 6; index++)
  {
    pinMode(pump_pins[index], OUTPUT);
    digitalWrite(pump_pins[index], HIGH);
  }
  for (unsigned char index = 0; index <= 4; index++)
  {
    pinMode(fan_pins[index], OUTPUT);
    digitalWrite(fan_pins[4], HIGH);
    setFansState(HIGH);
  }
  for (unsigned char index = 0; index <= 3; index++)
  {
    pinMode(valve_pins[index], OUTPUT);
    digitalWrite(valve_pins[index], HIGH);
  }
  Serial.begin(115200);
  while (!Serial);
  dht.begin();

  MsTimer2::set(100, operateProcess);
  MsTimer2::start();
  Display_Message();
}

void operateProcess()
{
  if (operateTimer)
  {
    timeCount--;
    if (timeCount <= 0)
    {
      waitProcess = 1;
      digitalWrite(13, LOW);
    }
  }
  serialTimeout++;
  if (serialTimeout >= 4)
  {
    readTDS(index); index++; if (index > 2) index = 0;
    Temp_Humidity();
    //Serial.println(serialData);
    serialTimeout = 0;
  }
}

unsigned int getMaxValue(unsigned int arr[], int size) 
{
  unsigned int maxVal = arr[0];  // Initialize maxVal with the first element

  for (int i = 1; i < size; i++) {
    if (arr[i] > maxVal) {
      maxVal = arr[i];  // Update maxVal if the current element is greater
    }
  }

  return maxVal;
}

void setFansState(int state) 
{
  for (int i = 0; i < 4; i++) 
  {
    digitalWrite(fan_pins[i], state);
  }
}

void Display_Message()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidity : ");
  lcd.setCursor(0,1);
  lcd.print("Temp : ");
  lcd.setCursor(0,2);
  lcd.print("TDS : ");
}


void waitTime(unsigned int delayTime)
{
  unsigned int wait = 0;
  for(wait = 0; wait<=delayTime; wait++)
  {
            readTDS(index); index++; if (index > 2) index = 0;
            Temp_Humidity();
            Display_System(humidity, temp, tds1, tds2, tds3);
  }
}

void loop()
{

  readTDS(index); index++; if (index > 2) index = 0;
  Temp_Humidity();

  if (doneS)
  {
    relaycontrol();
    doneS = 0;
    msgString = "";
  }

  if (operateTimer)
  {
    setFansState(LOW);
    Serial.println("all fans are on");
    waitTime(200);
    setFansState(HIGH);
    Serial.println("all fans are off");
    waitTime(50);
    if (Germination == 1)
    {
      for (gIndex = 0; gIndex <= 7; gIndex++) timingS[gIndex] = 0;
      timingS[0] = G1_TDS_TIME * (userTDS);
      timingS[4] = G2_TDS_TIME * (userTDS);
      timingS[5] = G3_TDS_TIME * (userTDS);
      timingS[6] = G4_TDS_TIME * (userTDS);
      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        Serial.print("G"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
      }
      maxTiming = getMaxValue(timingS, 8);
      Serial.print("Max Time : "); Serial.println(maxTiming);

      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        waitProcess = 0;
        timeCount = timingS[gIndex] / 10;
        if (timeCount > 0)
        {
          Serial.print("Relay ON: "); Serial.println(gIndex);
          while (waitProcess == 0)
          {
            digitalWrite(pump_pins[gIndex], LOW);
            waitTime(1);
          }
          digitalWrite(pump_pins[gIndex], HIGH);
          waitTime(10);
        }
      }
    }

    if (Flowering == 1)
    {
      for (gIndex = 0; gIndex <= 7; gIndex++) timingS[gIndex] = 0;
      timingS[1] = F1_TDS_TIME * (userTDS);
      timingS[4] = F2_TDS_TIME * (userTDS);
      timingS[5] = F3_TDS_TIME * (userTDS);
      timingS[6] = F4_TDS_TIME * (userTDS);
      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        Serial.print("F"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
      }
      maxTiming = getMaxValue(timingS, 8);
      Serial.print("Max Time : "); Serial.println(maxTiming);

      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        waitProcess = 0;
        digitalWrite(13, HIGH);

        timeCount = timingS[gIndex] / 10;
        if (timeCount > 0)
        {
          Serial.print("Relay ON: "); Serial.println(gIndex);
          while (waitProcess == 0)
          {
            digitalWrite(pump_pins[gIndex], LOW);
            waitTime(1);
          }
          digitalWrite(pump_pins[gIndex], HIGH);
          waitTime(10);
        }
      }
    }

    if (fruiting == 1)
    {
      for (gIndex = 0; gIndex <= 7; gIndex++) timingS[gIndex] = 0;
      timingS[2] = f1_TDS_TIME * (userTDS);
      timingS[4] = f2_TDS_TIME * (userTDS);
      timingS[5] = f3_TDS_TIME * (userTDS);
      timingS[6] = f4_TDS_TIME * (userTDS);
      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        Serial.print("f"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
      }
      maxTiming = getMaxValue(timingS, 8);
      Serial.print("Max Time : "); Serial.println(maxTiming);

      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        waitProcess = 0; digitalWrite(13, HIGH);

        timeCount = timingS[gIndex] / 10;
        if (timeCount > 0)
        {
          Serial.print("Relay ON: "); Serial.println(gIndex);
          while (waitProcess == 0)
          {
            digitalWrite(pump_pins[gIndex], LOW);
            waitTime(1);
          }
          digitalWrite(pump_pins[gIndex], HIGH);
          waitTime(10);
        }
      }
    }

    if (Boosting == 1)
    {
      for (gIndex = 0; gIndex <= 7; gIndex++) timingS[gIndex] = 0;
      timingS[3] = B1_TDS_TIME * (userTDS);
      timingS[4] = B2_TDS_TIME * (userTDS);
      timingS[5] = B3_TDS_TIME * (userTDS);
      timingS[6] = B4_TDS_TIME * (userTDS);
      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        Serial.print("B"); Serial.print(gIndex); Serial.print(" : "); Serial.println(timingS[gIndex]);
      }
      maxTiming = getMaxValue(timingS, 8);
      Serial.print("Max Time : "); Serial.println(maxTiming);

      for (gIndex = 0; gIndex <= 7; gIndex++)
      {
        waitProcess = 0; digitalWrite(13, HIGH);

        timeCount = timingS[gIndex] / 10;
        if (timeCount > 0)
        {
          Serial.print("Relay ON: "); Serial.println(gIndex);
          while (waitProcess == 0)
          {
            digitalWrite(pump_pins[gIndex], LOW);
            waitTime(1);
          }
          digitalWrite(pump_pins[gIndex], HIGH);
          waitTime(10);
        }
      }
    }
    Serial.println("Process Completed!");
    valvecontrol = 1;
    mixing = 1;
    valve = 1;
    operateTimer = 0; 
  }
  Display_System(humidity, temp, tds1, tds2, tds3);
  valve_control();
  mixing_fan_control();
}

void Display_System(unsigned int humidityS, unsigned int tempS, int tds1, int td2, int tds3)
{
  if(humidity>99)
  {
    lcd.setCursor(11,0);
    lcd.write(humidityS/100%10+48);
    lcd.write(humidityS/10%10+48);
    lcd.write(humidityS/1%10+48);
    lcd.print("%");
  }
  else
  {
    lcd.setCursor(11,0);
    lcd.write(humidityS/10%10+48);
    lcd.write(humidityS/1%10+48);
    lcd.print("%");
    lcd.print(" ");
  }

  lcd.setCursor(7,1);  
  lcd.print(tempS);
  lcd.print((char)223);
  lcd.print("C");
  
  lcd.setCursor(0,3);
  lcd.print("A-");
  lcd.write(tds1/100%10+48);
  lcd.write(tds1/10%10+48);
  lcd.write(tds1/1%10+48);
  lcd.print("  B-");
  lcd.write(tds2/100%10+48);
  lcd.write(tds2/10%10+48);
  lcd.write(tds2/1%10+48);
  lcd.print("  C-");
  lcd.write(tds3/100%10+48);
  lcd.write(tds3/10%10+48);
  lcd.write(tds3/1%10+48);
  delay(10);
}

void serialEvent()
{
  if (Serial.available())
  {
    char c = Serial.read();
    if (c == '#') {
      msgStart = 0;
      doneS = 1;
    }
    if (msgStart) msgString += c;
    if (c == '*') {
      msgStart = 1;
      msgString = "";
      doneS = 0;
    }
  }
}

void relaycontrol()
{
  if (finalTDS < 100)
  {
    userTDS = (msgString[1] - 48) * 1000 + (msgString[2] - 48) * 100 + (msgString[3] - 48) * 10 + (msgString[4] - 48);
    valvecontrol = (msgString[6] - 48);
    if (msgString[0] == 'G')
    {
      Serial.println("MSG = " + msgString);
      operateTimer = 1; Germination = 1; Flowering = 0; fruiting = 0; Boosting = 0;
    }
    if (msgString[0] == 'F')
    {
      Serial.println("MSG = " + msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 1; fruiting = 0; Boosting = 0;
    }
    if (msgString[0] == 'f')
    {
      Serial.println("MSG = " + msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 0; fruiting = 1; Boosting = 0;
    }
    if (msgString[0] == 'B')
    {
      Serial.println("MSG = " + msgString);
      r1ONTime = 5; r2ONTime = 3; r3ONTime = 2; r4ONTime = 0; r5ONTime = 0;
      operateTimer = 1; Germination = 0; Flowering = 0; fruiting = 0; Boosting = 1;
    }
  }
}

void readTDS(unsigned char channel) {
  float tdsSum = 0.0, tdsValue = 0.0;
  int sensorValue = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sensorValue = analogRead(channel);
    tdsValue = sensorValue * (5.0 / 1023.0) * (1000.0 / 2.5);
    tdsSum += tdsValue;
  }
  averageTDS = (unsigned int)(tdsSum / NUM_SAMPLES);

  switch (channel)
  {
    case 0: serialData[T1_Address]   = averageTDS / 1000 % 10 + 48;
      serialData[T1_Address + 1] = averageTDS / 100 % 10 + 48;
      serialData[T1_Address + 2] = averageTDS / 10 % 10 + 48;
      serialData[T1_Address + 3] = averageTDS / 1 % 10 + 48;
      tds1 = averageTDS;
      break;
    case 1: serialData[T2_Address]   = averageTDS / 1000 % 10 + 48;
      serialData[T2_Address + 1] = averageTDS / 100 % 10 + 48;
      serialData[T2_Address + 2] = averageTDS / 10 % 10 + 48;
      serialData[T2_Address + 3] = averageTDS / 1 % 10 + 48;
      tds2 = averageTDS;
      break;
    case 2: serialData[T3_Address]   = averageTDS / 1000 % 10 + 48;
      serialData[T3_Address + 1] = averageTDS / 100 % 10 + 48;
      serialData[T3_Address + 2] = averageTDS / 10 % 10 + 48;
      serialData[T3_Address + 3] = averageTDS / 1 % 10 + 48;
      tds3 = averageTDS;
      break;
  }

}

void Temp_Humidity()
{
  
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  serialData[H_Address]    = humidity / 100 % 10 + 48;
  serialData[H_Address + 1]  = humidity / 10 % 10 + 48;
  serialData[H_Address + 2]  = humidity / 1 % 10 + 48;

  serialData[T_Address]    = temp / 100 % 10 + 48;
  serialData[T_Address + 1]  = temp / 10 % 10 + 48;
  serialData[T_Address + 2]  = temp / 1 % 10 + 48;
}

void valve_control()
{
  if (flag == 1)
  {
    if (valve == 1)
    {
      if (valvecontrol == 1) {
        digitalWrite(valve_pins[1], LOW);
        Serial.println("valve 1 is on");
        waitTime(200);
        digitalWrite(valve_pins[1], HIGH);
        Serial.print("valve 1 is off");
        waitTime(100);
      }
      else if (valvecontrol == 2) {
        digitalWrite(valve_pins[2], LOW);
        Serial.println("valve 2 is on");
        waitTime(200);
        digitalWrite(valve_pins[2], HIGH);
        Serial.println("valve 2 is off");
        waitTime(100);
      }
    } valve = 0;  flag = 0;
  }
}


void mixing_fan_control()
{
  if (mixing == 1) {
    digitalWrite(fan_pins[0], LOW);
    Serial.println("mixing fan is started");
    waitTime(100);
    digitalWrite(fan_pins[0], HIGH);
    Serial.println("mixing fan is stopped");
    waitTime(20);
    mixing = 0;
    flag = 1;
  }
}
