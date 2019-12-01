/*
 * WS2801 RGB String controller with:
 * MQTT, and ARTnet for control with XLights
 * All Tech Plus, 2019
 */
 
#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>

#include <ESP8266mDNS.h>

#include <Artnet.h>
#include <RingBuffer.h>

#include <FastLED.h>
FASTLED_USING_NAMESPACE
#include "settings.h"

CRGB leds[numLeds];

#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <ESPAsyncWiFiManager.h>

#ifdef ENABLE_OTA
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
#endif
#ifdef ENABLE_MQTT
  #include <PubSubClient.h>
#endif

// MQTT
#ifdef ENABLE_MQTT
  #include <PubSubClient.h>
  #ifdef ENABLE_HOMEASSISTANT
    #include <ArduinoJson.h>
  #endif
// setup device network name

  WiFiClient espClient;
  PubSubClient mqtt_client(espClient);
#endif

AsyncWebServer server(80);
AsyncWebServer serverap(8080);
DNSServer dns;

ArtnetReceiver artnet;

void callback(uint8_t* data, uint16_t size)
{
    // you can also use pre-defined callbacks
    Serial.println("callback...");
}

void reboot() {
  // seems like resetting serial will bring about a restart
  Serial.begin(57600);
  delay(pause);
  lightsoff();
  Serial.println("Controller restarted.");
}
void setup() {
  // setup serial debug port
  Serial.begin(57600);
  Serial.println("Booting...");
  checkConfig();
  getversion();
  // set built in LED which will flash when receiving artnet packets 
  pinMode(BUILTIN_LED,OUTPUT); 
  // setup pin for ondemandwifi
  ondemandsetup();
  // setup the wifi or start ondemandap
  checkwifi(); 
  wifi_station_set_hostname(const_cast<char*>(HOSTNAME));
  websetup();

  DBG_OUTPUT_PORT.print("Open http://");
  DBG_OUTPUT_PORT.print(WiFi.localIP());
  DBG_OUTPUT_PORT.println("/ to open Arnet Controller.");

  DBG_OUTPUT_PORT.print("Use http://");
  DBG_OUTPUT_PORT.print(HOSTNAME);
  DBG_OUTPUT_PORT.println(".local/ when you have Bonjour installed.");

 // ***************************************************************************
  // Setup: MDNS responder
  // ***************************************************************************
  wifi_station_set_hostname(const_cast<char*>(HOSTNAME));
  bool mdns_result = MDNS.begin(HOSTNAME);
  if (!mdns_result) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
  MDNS.update();
  Serial.println("mDNS responder started");
  otasetup();
  Serial.print("The ledtype is "); Serial.println(ledtype);
  switch (ledtype) {
    case 0: // ws2801 using clock pin
      Serial.println("WS2801 string in use (with clock).");
      FastLED.addLeds<WS2801,DATA_PIN,CLOCK_PIN,RGB>(leds, numLeds);
      break;
    case 1: // ws2811 using data pin only
      Serial.println("WS2811 string in use (no clock).");
      FastLED.addLeds<WS2811,DATA_PIN,RGB>(leds, numLeds);
      break;
    default:
      Serial.println("Oh dear I'm not sure what pixels you are using.");
      break;
  }
  Serial.println("Setting up MQTT");
  mqtt_client.setServer("192.168.1.131", 1883);
  Serial.print("Listening for Artnet/DMX on universe "); Serial.println(myuniverse);

  artnet.begin(); // waiting for Art-Net in default port
  lightsoff();
  FastLED.show();


  

  
  artnet.subscribe(myuniverse, [&](uint8_t* data, uint16_t size) {
  // using timera and timeb to see how long we are without a packet
  // if it is a long time and autoeffects are enabled we will reinstate
  // the autoeffects
  timera = millis();
// sanity check data - if its exactly the same drop the frame
for (int check=0; check<numberOfChannels; ++check) {
  mychecksum=data[check]+mychecksum;
}
  // packets being received so turn off startup effects or any selected effect that was in force
  autoeffects = -1;
  effectselected = -1;
if (lastchecksum != mychecksum) { 
   //toggle state of built in LED to show new packet received
  int state = digitalRead(BUILTIN_LED);  // get the current state of on board led
  // Serial.println(state); // I used this to check to see if the onboard LED status was changing
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
  // do the lights
  lastchecksum=mychecksum;
  // loop through the data array, 3 numbers per channel
  for (int lamp = 0; lamp<NUM_LEDS; ++lamp)
   {
    leds[lamp] = CRGB(data[lamp * 3],data[lamp * 3 +1], data[lamp * 3 +2]);
   }
   //Serial.println(mychecksum); // I used this to debug if it was a new packet
   FastLED.show();
}

  
});
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, RunningLightsRed, RunningLightsWhite, RunningLightsBlue,NewKITTStub, theaterChaseRainbowStub };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns



void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("artnet1", "artnet1", "artnet1")) {
      Serial.println("MQQT connected");
      // Once connected, publish an announcement...
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
MDNS.update();
dns.processNextRequest();
ArduinoOTA.handle();
  // put your main code here, to run repeatedly:
artnet.parse(); // check if artnet packet has come and execute callback
timerb = millis();
if (((timerb - timera) > 10000) && ((timerb - timera) < 15000)) {
  // if we have been processing Artnet/DMX and nothing for 10 seconds
  // reload the config to reinstate autoeffects if they are turned on
  Serial.println("No more Artnet packets.");
  loadConfig();
  lightsoff();
  digitalWrite(BUILTIN_LED, true);
  // delay long enough so we don't get multiple reloads of the config
  delay(pause);
}
 if (autoeffects == 1 && randomeffects == 1) {
 // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  artnet.parse(); // check if artnet packet has come and execute callback

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

// do some periodic updates
  if (autoeffects != 0) {
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  }
 } else {
  EVERY_N_MILLISECONDS( 5 ) { artnet.parse(); }
 }

 if (autoeffects == 1 && randomeffects !=1) {
 // Call the current pattern function once, updating the 'leds' array
  gPatterns[selectedeffectno]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  if (autoeffects != 0) {
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  }
  }


}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  artnet.parse(); // check if artnet packet has come and execute callback
}
