#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>

// prototypes
boolean connectWifi();

//callback functions
//void firstLightChanged(uint8_t brightness);
void firstLightChanged(EspalexaDevice* dev);
         
// define the GPIO connected with Relays
const int ledPin_1 = 13; // the PWM pin the LED is attached to
const int potPin   = 34; // Potentiometer Pin
const int wifiLed  = 32; // Wifi indicator LED Pin

// WiFi Credentials
const char* ssid = "Wifi Name";
const char* password = "Wifi Password";

// device names
String Device_1_Name = "Studio lights";

// setting PWM properties
const int freq = 2000;
const int ledChannel_0 = 0;
const int resolution = 8;

uint8_t brightness;
int potValue = 0;

boolean wifiConnected = false;

Espalexa espalexa;

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
  espalexa.addDevice(Device_1_Name, firstLightChanged, EspalexaDeviceType::dimmable, 127); //Dimmable device, optional 4th parameter is beginning state (here fully on)
  espalexa.begin();
}

//our callback functions
void firstLightChanged(EspalexaDevice* d) {
  if (d == nullptr) return;

  brightness = d->getValue();
  Serial.print(brightness);

  ledcWrite(ledChannel_0, brightness);
  delay(15);
}

void setup()
{
  Serial.begin(115200);
  pinMode(potPin, INPUT);
  pinMode(wifiLed, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel_0, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin_1, ledChannel_0);
  
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
    Serial.print("WiFi Not Connected ");
    Serial.println(wifiConnected);
    digitalWrite(wifiLed, LOW);
    
    potValue  = analogRead(potPin);
    brightness = map(potValue, 0, 4095, 0, 255);
    
    ledcWrite(ledChannel_0, brightness);
    delay(15);
    
    Serial.print(potValue);
    Serial.print("  ");
    Serial.println(brightness);
  }
  else
  {
    Serial.print("WiFi Connected  ");
    Serial.println(wifiConnected);
    digitalWrite(wifiLed, HIGH);
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
}
