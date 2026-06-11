/**
 * Aqara camera IFTTT interface
 * Name: aqara-camera-ifttt-interface
 * Purpose: This interface allows using the AI functions of the Aqara Camera, such as face detection, as a trigger for IFTTT applets.
 * 
 * @author Till Maria Jürgens
 * @version 0.1.0
 * 
 * This project is developed and used in a student project by the MOVEN research group at the chair of Design Digital Media, Rhine-Waal University of Applied Sciences. More information: https://www.sustainabilitybydesign.net/moven
*/

// include libraries
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <IRremote.hpp>

// ID of virtual device, 4 stands for a LG TV
#define IR_REMOTE_ID 4

// vars
String ssid;
String password;
String ifttt_key;
String ifttt_event_prefix;

// pins
#define SD_PIN          16
#define IR_RECEIVE_PIN  17

// IFTTT connection
WiFiClient client;
#define HOST "maker.ifttt.com"
#define PORT 80


/*
Pseudo-decode of commands from the number buttons 0 – 9 that are coded into 16 – 25
*/
String decode_command(int command) {
  int command_decoded = command - 16;
  return String(command_decoded);
}

/*
Call IFTTT trigger with JSON data
*/
void send_webhook(int address, int command) {
  if (address != IR_REMOTE_ID) {
    return;
  }

  if (!client.connect(HOST, PORT)) {
    return;
  }

  // construct JSON
  String jsonString = "";
  jsonString += "{\"value1\":\"";
  jsonString += String(address);
  jsonString += "\",\"value2\":\"";
  jsonString += String(command);
  jsonString += "\",\"value3\":\"";
  jsonString += decode_command(command);
  jsonString += "\"}";
  int jsonLength = jsonString.length();  
  String lenString = String(jsonLength);

  // construct POST request
  String postString = "";
  postString += "POST /trigger/";
  postString += ifttt_event_prefix + decode_command(command);
  postString += "/with/key/";
  postString += ifttt_key;
  postString += " HTTP/1.1\r\n";
  postString += "Host: maker.ifttt.com\r\n";
  postString += "Content-Type: application/json\r\n";
  postString += "Content-Length: ";
  postString += lenString + "\r\n";
  postString += "\r\n";
  postString += jsonString;
  postString += "\r\n";

  Serial.println(postString);

  client.print(postString);
  delay(500);
  client.stop();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);

  /*
  Init SD card
  and get WiFi and IFTTT credentials
  */
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  // read file
  File file = SD.open("/wifi.txt");
  if(!file){
    Serial.println("Failed to open file");
    return;
  }

  // parse credentials, format "ssid,password,ifttt_key"
  String content = "";
  while(file.available()){
      content += (char)file.read();
  }
  file.close();
  int firstComma  = content.indexOf(',');
  int secondComma = content.indexOf(',', firstComma + 1);
  int thirdComma = content.indexOf(',', secondComma + 1);
  ssid = content.substring(0, firstComma);
  password = content.substring(firstComma + 1, secondComma);
  ifttt_event_prefix = content.substring(secondComma + 1, thirdComma);
  ifttt_key = content.substring(thirdComma + 1);
  
  /*
  Init WiFi
  and connect
  */
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;
  bool wifiConnected = false;

  while (true) {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true;
        break;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    if (wifiConnected) break;
    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      WiFi.disconnect();
      break;
    } else {
      numberOfTries--;
    }
  }

  if (!wifiConnected) {
    while (true) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
  
  /*
  Init IrReceiver
  */
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("test: ");
    Serial.print(IrReceiver.decodedIRData.address);
    Serial.print(" # ");
    Serial.println(IrReceiver.decodedIRData.command);
    IrReceiver.resume(); // early enable receiving of the next IR frame
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) { // prevent multiple signals in short time
      Serial.println(F("Repeat received. Do nothing and wait for next input."));
    } else {
      send_webhook(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
    }
  }
  
  delay(10);
}
