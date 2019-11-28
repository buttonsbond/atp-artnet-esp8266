// these have to be hard coded, least wise I couldn't suss out how to change them via. config
// maybe someone with more knowledge than me might be able to suss this?
// the 2 int below aren't used, they are stored in the config file for future use to address the
// above issue!
int dp = 5;
int cp = 6;
#define CLOCK_PIN 6  // WS2801 Data pin  D6 . gpio 14
#define DATA_PIN 5   // WS2801 Clock pin D5 .  gpio 12
#define BUILTIN_LED 2

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

#define ENABLE_OTA      // comment out if you don't want OTA updates
int OTAport = 8266;
//const char* OTAPassword = "artnet";
char HOSTNAME[20] = "ARTNET-DEFAULT";   // Friendly hostname (todo: generate a unique hostname so don't have to set on per board basis
char NODENAME[20] = "ARTNET-DEFAULT"; // i've used NODENAME in the HTML files
//
// these array strings are used with the UI in checking the previously selected item
//
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

//
// artnet/dmx default settings
//
uint32_t myuniverse = 1;
//
// LED String vars
//
int ledtype = 0; // 0 is WS2801 , 1 is WS2811 - you can change this through config anyhow
String LED_TYPE = "WS2801";
int NUM_LEDS = 100; // can also be changed in config
const int numLeds = 170; // max channels in a DMX universe is 512, 3 channels per pixel so max leds is actually 170
int numberOfChannels = NUM_LEDS * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
//
// these vars are used to check DMX packets coming in, there are usually quite a few repeated packets
// basically if its the same as the last packet FASTLED won't take any further action
//
int mychecksum = 0;
int lastchecksum = 0;
//
// automatic effects on startup
//
int autoeffects = 0; // if 0 the board will startup with lights off
int randomeffects = 0; // if set to 1 it will allow random effects on startup if autoeffects also 1
int effectselected = 0; // FOR FUTURE USE
int selectedeffectno = 0; // this is the effect number selected from the web interface or mqtt

unsigned long timera = millis();
unsigned long timerb = millis();


//const char* ssid = "Telfy_3316";
char ssid[] = "Telfy_3316";
//const char* password =  "jonesxa1";
char password[] = "jonesxa1";


#define DBG_OUTPUT_PORT Serial  // Set debug output port

int pause = 5000; // some delays are added when parameters are saved in an attempt to stop the web server barfing
