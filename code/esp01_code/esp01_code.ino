/*
 This Project is developed by sanghani Krish.
 En no:- 216020311008
 Starting developing at 05/08/2023
 Uplode this code to esp01
 connect your wifi with SSID:-krish
 and use password:- krish@231

*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "krish";
const char *password = "krish@231";

ESP8266WebServer server(80);
bool resultPrinted = false;
void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    delay(100);

    if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

    Serial.println("");
    server.on("/", HTTP_GET, handleRoot);
    server.on("/result",handleResult);
    server.on("/upload", HTTP_POST, handleRootIcon);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

void handleRoot() {
    String content = "<html><head><title>Nutrition TDS</title></head><body>";
    content += "<h1>Nutrition TDS</h1>";
    content += "<form action='/result' method='GET'>";
    content += "Nutrition TDS Value: <input type='text' name='nutrition_tds'><br>";
    content += "<input type='checkbox' name='G'> Germination<br>";
    content += "<input type='checkbox' name='F'> Flowering<br>";
    content += "<input type='checkbox' name='f'> Fruiting<br>";
    content += "<input type='checkbox' name='B'> Boosting<br>";
    content += "<input type='submit' value='Submit'>";
    content += "</form>";
    content += "</body></html>";

    server.send(200, "text/html", content);
}

void Send_Serial_Argument(unsigned char arg, unsigned int valueS)
{
  Serial.print('*');
  switch(arg)
  {
    case 'G': 
              Serial.print("G"); break;
    case 'F':
              Serial.print('F'); break;
    case 'f':
              Serial.print('f'); break;
    case 'B':
              Serial.print('B'); break;
              
  }
  Serial.print(valueS/1000%10); Serial.print(valueS/100%10); Serial.print(valueS/10%10); Serial.print(valueS/1%10);
  Serial.print('#');
}

void handleRootIcon() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>ESP8266 Web Server - Upload Icon</title>";
  html += "</head><body>";
  html += "<h1>Upload Icon</h1>";
  html += "<form action='/upload' method='post' enctype='multipart/form-data'>";
  html += "<input type='file' name='icon' accept='image/png'>";
  html += "<input type='submit' value='Upload'>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/icon.png";
    File file = LittleFS.open(filename, "w");
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }
    Serial.print("Uploading: ");
    Serial.println(filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Serial.print(".");
    if (upload.currentSize > 1024) { // Max file size check (adjust as needed)
      Serial.println("File size exceeds limit");
      upload.abort();
    } else {
      File file = LittleFS.open("/icon.png", "a");
      if (!file) {
        Serial.println("Failed to open file for writing");
        return;
      }
      file.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    Serial.println("Upload finished successfully");
  }
}



void handleResult()
{
        if(server.arg("nutrition_tds").toInt()>1500)
        {
          /*
          String content = "<html><head><title>Nutrition TDS</title></head><body>";
          content += "<h1>Invalid Nutrition TDS</h1>";
          content += "<h1>Enter TDS Value Between 1 to 1500...</h1>";
          server.send(200,"text/html",content);
          delay(2000);
          */
          String alert = "<script> alert('Please enter Nutrition TDS value between 0 and 1500'); window.location='/'; </script>";
          server.send(400, "text/html", alert);
        }
        else
        {
          if (server.hasArg("G")) {
           Send_Serial_Argument('G',server.arg("nutrition_tds").toInt()); 
          }
          if (server.hasArg("F")) {
              Send_Serial_Argument('F',server.arg("nutrition_tds").toInt());
          }
           if (server.hasArg("f")) {
              Send_Serial_Argument('f',server.arg("nutrition_tds").toInt());
          }
           if (server.hasArg("B")) {
              Send_Serial_Argument('B',server.arg("nutrition_tds").toInt());
          }
          server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
          server.send(303); 
        }
        
}


  
