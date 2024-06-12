#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2          
#define DHTTYPE DHT11     

LiquidCrystal_I2C lcd(0x27, 16, 2);  
DHT dht(DHTPIN, DHTTYPE);

int RELAY_3 = 2;
int RELAY_4 = 3;
int RELAY_6 = 4;
int RELAY_7 = 5;
int RELAY_8 = 6;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RELAY_6, OUTPUT);
  pinMode(RELAY_7, OUTPUT);
  pinMode(RELAY_8, OUTPUT);
  digitalWrite(RELAY_3,HIGH);
  digitalWrite(RELAY_4,HIGH);
  digitalWrite(RELAY_6,HIGH);
  digitalWrite(RELAY_7,HIGH);
  digitalWrite(RELAY_8,HIGH);
}

void loop() {
  /*float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.print("DHT11 Error");
    return;
  }

  lcd.clear();
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  delay(100);*/
  relaycontrol();

}
  void relaycontrol() {
   if (Serial.available() > 0) { 
    String data = Serial.readStringUntil('\n');
    int value = data.toInt(); 
    
    if (data >= * && data <= #) {
      Serial.print("gerdone");
      }
    }
  }


  /*if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(RELAY_3, HIGH);
      Serial.println("realy 3 is high");
      delay(5000);
      digitalWrite(RELAY_3, LOW);
      Serial.println("realy 3 is low");
      delay(4000);
      digitalWrite(RELAY_6, HIGH);
      Serial.println("realy 6 is high");
      delay(5000);
      digitalWrite(RELAY_6, LOW);
      Serial.println("realy 6 is low");
      delay(1000);
      digitalWrite(RELAY_7, HIGH);
      Serial.println("realy 3 is low");
      delay(3000);
      digitalWrite(RELAY_7, LOW);
      Serial.println("realy 7 is low");
      delay(1000);
      digitalWrite(RELAY_8, HIGH);
      Serial.println("realy 8 is high");
      delay(2000);
      digitalWrite(RELAY_8, LOW);
      Serial.println("realy 8 is low");
    }
    else if (command == '2') {     
      digitalWrite(RELAY_3, HIGH);
      Serial.println("realy 3 is high");
      delay(12000);
      digitalWrite(RELAY_3, LOW);
      Serial.println("realy 3 is low");
      delay(1000);
      digitalWrite(RELAY_6, HIGH);
      Serial.println("realy 6 is high");
      delay(5200);
      digitalWrite(RELAY_6, LOW);
      Serial.println("realy 6 is low");
      delay(1000);
      digitalWrite(RELAY_7, HIGH);
      Serial.println("realy 7 is high");
      delay(3200);
      digitalWrite(RELAY_7, LOW);
      Serial.println("realy 7 is low");
      delay(1000);
      digitalWrite(RELAY_8, HIGH);
      Serial.println("realy 8 is high");
      delay(2300);
      digitalWrite(RELAY_8, LOW);
      Serial.println("realy 8 is low");
    }
    else if (command == '3') {
      digitalWrite(RELAY_4, HIGH);
      delay(10000);
      digitalWrite(RELAY_4, LOW);
      delay(1000);
      digitalWrite(RELAY_6, HIGH);
      delay(5000);
      digitalWrite(RELAY_6, LOW);
      delay(1000);
      digitalWrite(RELAY_7, HIGH);
      delay(3000);
      digitalWrite(RELAY_7, LOW);
      delay(1000);
      digitalWrite(RELAY_8, HIGH);
      delay(2000);
      digitalWrite(RELAY_8, LOW);
    }
    else if (command == '4') {
      digitalWrite(RELAY_4, HIGH);
      delay(10200);
      digitalWrite(RELAY_4, LOW);
      delay(1000);
      digitalWrite(RELAY_6, HIGH);
      delay(5200);
      digitalWrite(RELAY_6, LOW);
      delay(1000);
      digitalWrite(RELAY_7, HIGH);
      delay(3300);
      digitalWrite(RELAY_7, LOW);
      delay(1000);
      digitalWrite(RELAY_8, HIGH);
      delay(2100);
      digitalWrite(RELAY_8, LOW);
    }
  }*/
