#define ENABLE_OTA      // comment out if you don't want OTA updates
int OTAport = 8266;
//const char* OTAPassword = "artnet";
char HOSTNAME[20] = "ARTNET-DEFAULT";   // Friendly hostname (todo: generate a unique hostname so don't have to set on per board basis
char NODENAME[20] = "ARTNET-DEFAULT"; // i've used NODENAME in the HTML files
String selected[20];
String selectedled[2];
String startupeffectsenabled[2];
String cycleeffects[2];

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//define FASTLED_ESP8266_D1_PIN_ORDER

#define ENABLE_MQTT

// these defines are used for just running random animations when there's no show playing
int BRIGHTNESS     =     96;
int FRAMES_PER_SECOND = 240;

// LED Strip
int ledtype = 0;
int NUM_LEDS = 100;
const int numLeds = 170; // max channels in a DMX universe is 512, 3 channels per pixel so max leds is actually 170
int numberOfChannels = NUM_LEDS * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
int mychecksum = 0;
int lastchecksum = 0;
// if this is set to 1 then when the controller fires up the lights will either cycle preset effects or the previously 1 selected effect
int autoeffects = 0; 
int randomeffects = 0; // if set to 1 it will allow random effects on startup if autoeffects is set to -1
int effectselected = 0; // this value will be set to 1 if an effect is selected or chosen from web interface or mqtt
int selectedeffectno = 0; // this is the effect number selected from the web interface or mqtt

// these have to be hard coded, least wise I couldn't suss out how to change them via. config
// maybe someone with more knowledge than me might be able to suss this?
// the 2 int below aren't used, they are stored in the config file for future use to address the
// above issue!
int dp = 5;
int cp = 6;
#define CLOCK_PIN 6  // WS2801 Data pin  D6 . gpio 14
#define DATA_PIN 5   // WS2801 Clock pin D5 .  gpio 12
#define BUILTIN_LED 2

uint32_t universe1 = 1;
uint32_t universe2 = 2;
uint32_t myuniverse = 1;

//const char* ssid = "Telfy_3316";
char ssid[] = "Telfy_3316";
//const char* password =  "jonesxa1";
char password[] = "jonesxa1";

String LED_TYPE = "WS2801";

#define DBG_OUTPUT_PORT Serial  // Set debug output port
