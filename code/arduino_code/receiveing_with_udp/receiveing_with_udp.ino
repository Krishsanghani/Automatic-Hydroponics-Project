#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ESP_AP"; 
const char* password = "password123"; 
const int LED  = 2;

WiFiUDP udp;

const int localPort = 8888;

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  udp.begin(localPort);
  Serial.println("UDP Started");
}

void relaycontrol(String data) {
  int value = data.toInt(); 
    
  if (value >= 1 && value <= 100) {
    digitalWrite(LED,value);
  }
}

void loop() {
  char packetBuffer[255]; 
  int packetSize = udp.parsePacket(); 
  if (packetSize) {
    udp.read(packetBuffer, 255); 
    String receivedData = String(packetBuffer);
    relaycontrol(receivedData); 
  }
  delay(1000); 
}

