/**********************************************************************************
    TITLE: ESP32 Home Automation project using Blynk | Internet & Manual with Realtime Feedback
    Click on the following links to learn more.
    YouTube Video: https://youtu.be/zCoEe1t1_QY
    Related Blog : https://easyelectronicsproject.com/esp32-projects/
    by Tech StudyCell
 **********************************************************************************/
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

 *************************************************************
  Update the Preferences –> Aditional boards Manager URLs:
  https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json

  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)

 *************************************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

int toggleState_1 = 1;
//int pushButton1State = HIGH;

//int toggleState_2 = 1;
//int pushButton2State = HIGH;

int wifiFlag = 0;

#define AUTH "wvYkclKVaHb0Z7VM4zTPWQKKo6aA98hm"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "Xiaomi_D159"             //Enter Wifi Name
#define WIFI_PASS "A5%Jimish"         //Enter wifi Password

#define RELAY_PIN_1      15   //D26
#define WIFI_LED         2   //D25

//#define PUSH_BUTTON_1    32   //D32

#define VPIN_BUTTON_1    V1

void relayOnOff(int relay) {

  switch (relay) {
    case 1:
      if (toggleState_1 == 0) {
        digitalWrite(RELAY_PIN_1, HIGH); // turn on relay 1
        toggleState_1 = 1;
      }
      else {
        digitalWrite(RELAY_PIN_1, LOW); // turn off relay 1
        toggleState_1 = 0;
      }
      delay(200);
      break;
    default : break;
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RELAY_PIN_1, toggleState_1);
}

//void with_internet() {
//  if (digitalRead(PUSH_BUTTON_1) == LOW) {
//    relayOnOff(1);
//    // Update Button Widget
//    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
//  }
//}
//void without_internet() {
//  if (digitalRead(PUSH_BUTTON_1) == LOW) {
//    relayOnOff(1);
//  }
//}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(WIFI_LED, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    digitalWrite(WIFI_LED, HIGH);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN_1, OUTPUT);
//  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, toggleState_1);

  pinMode(WIFI_LED, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not Connected");
  }
  else
  {
    Serial.println(" Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    Serial.println(" with internet");
//    with_internet();
  else
    Serial.println(" without internet");
//    without_internet();
}
