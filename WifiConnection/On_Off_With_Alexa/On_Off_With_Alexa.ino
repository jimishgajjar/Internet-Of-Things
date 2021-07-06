#include <WiFi.h>
#include <Espalexa.h>
#include <AceButton.h>
using namespace ace_button;

Espalexa espalexa;

// define the GPIO connected with Relays and switches
#define RelayPin1 15  //D23

#define wifiLed    2   //D2

// WiFi Credentials
const char* ssid = "Xiaomi_D159";
const char* password = "A5%Jimish";

// device names
String Device_1_Name = "Study Lamp";

// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);

ButtonConfig config1;
AceButton button1(&config1);

void handleEvent1(AceButton*, uint8_t, uint8_t);
boolean wifiConnected = false;

//our callback functions
void firstLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, LOW);
      Serial.println("Device1 ON");
    }
  else
  {
    digitalWrite(RelayPin1, HIGH);
    Serial.println("Device1 OFF");
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}

void addDevices(){
  // Define your devices here.
  espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off

  espalexa.begin();
}

void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);

  pinMode(wifiLed, OUTPUT);
  

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);

  config1.setEventHandler(button1Handler);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    addDevices();
  }
  else
  {
    Serial.println("Cannot connect to WiFi. So in Manual Mode");
    delay(1000);
  }
}

void loop()
{
   if (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print("WiFi Not Connected ");
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  else
  {
    //Serial.print("WiFi Connected  ");
    digitalWrite(wifiLed, HIGH);
    //Manual Switch Control
    //WiFi Control
    if (wifiConnected){
      espalexa.loop();
      delay(1);
    }
    else {
      wifiConnected = connectWifi(); // Initialise wifi connection
      if(wifiConnected){
      addDevices();
      }
    }
  }
  button1.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  EspalexaDevice* d1 = espalexa.getDevice(0); //this will get "first device", the index is zero-based
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      d1->setPercent(100); //set value "brightness" in percent
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      d1->setPercent(0); //set value "brightness" in percent
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}
