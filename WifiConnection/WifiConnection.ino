#define led 2

#include "WiFi.h"

  
const char* ssid = "Xiaomi_D159";
const char* password =  "A5%Jimish";

void setup() {
 Serial.print("Connecting to wifi");
 WiFi.mode(WIFI_STA);
 Serial.begin(115200);
 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  pinMode(led, OUTPUT);
}
 
void loop() {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
