#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266Ping.h>

const char* ssid = "ESP8266";
const char* password = "123456789";

//Your IP address or domain name with URL path
const char* serverNameTemp = "http://192.168.4.1/mesaj1";
const char* serverNameHumi = "http://192.168.4.1/mesaj2";
IPAddress ip(192,168,4,1); //ip adress of the server

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long previousMillis = 0;
const long interval = 5000; 
String m1;
String m2;

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  // Address 0x3C for 128x64, you might need to change this value (use an I2C scanner)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
 
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("connecting...");
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.display();  
  }
  Serial.println("");
  Serial.println("Connected to WiFi  with IP Address:");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("connected!");
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if ((WiFiMulti.run() == WL_CONNECTED)) {
      bool ret1 = Ping.ping(ip,10); // ping 10 times
      m1 = httpGETRequest(serverNameM1);
      m2 = httpGETRequest(serverNameM2);
      Serial.println("Connected to WiFi!");
      
      display.clearDisplay();
      
       // display 1st message
      display.setTextSize(2);
      display.setCursor(0,0);
      display.print("Average response time (10 pings): ");
      display.print(Ping.averageTime());
      display.print(" ");
      display.print("ms");
      display.setTextSize(1);
      display.cp437(true);
      
      // display 2nd message
      display.setTextSize(1);
      display.setCursor(0, 25);
      display.print(m2);
      
      display.display(); // displays every change
      
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
