struct Config {
  char* host;
  char* ssid;
  char* pw;
  char* mqtt;
  int ledtype;
  int datapin;
  int clockpin;
  int autoeffects;
  int randomeffects;
  int selectedeffectno;
  int effectselected;
  int brightness;
  int fps;
  int numleds;
  int universe;
};

Config config;
myversion vr;

// Saves the configuration to a file
void saveConfiguration() {
  // Delete existing file, otherwise the configuration is appended to the file
  //SD.remove(filename);
Serial.print("Autoeffects: "); Serial.println(autoeffects);
Serial.print("Randomeffects: "); Serial.println(randomeffects);
Serial.print("Single Effect: "); Serial.println(selectedeffectno);

  // Open file for writing
  File file = SPIFFS.open("/config.json", "w");
  if (!file) {
    Serial.println("Failed to create config file");
    return;
  }

  config.host=HOSTNAME;
  config.ssid=ssid;
  config.pw=password;
  config.mqtt="192.168.1.131"; // need to use a variable later
  config.ledtype=ledtype;
  config.datapin=DATA_PIN;
  config.clockpin=CLOCK_PIN;
  config.autoeffects=autoeffects;
  config.randomeffects=randomeffects;
  config.selectedeffectno=selectedeffectno;
  config.effectselected=effectselected;
  config.brightness=BRIGHTNESS;
  config.fps=FRAMES_PER_SECOND;
  config.numleds=NUM_LEDS;
  config.universe=myuniverse;
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  //DynamicJsonDocument doc(1024);
  //StaticJsonDocument<256> doc;
  StaticJsonBuffer<1024> jsonBuffer;
  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();
  root["hostname"] = config.host;
  root["ssid"] = config.ssid;
  root["psk"] = config.pw;
  root["mqtt"] = config.mqtt;
  root["ledtype"] = config.ledtype;
  root["datapin"] = config.datapin;
  root["clockpin"] = config.clockpin;
  root["autoeffects"] = config.autoeffects;
  root["randomeffects"] = config.randomeffects;
  root["selectedeffectno"] = config.selectedeffectno;
  root["effectselected"] = config.effectselected;
  root["brightness"] = config.brightness;
  root["fps"] = config.fps;
  root["numleds"] = config.numleds;
  root["universe"] = config.universe;
  

 // Serialize JSON to file
  if (root.printTo(file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
  resetdropdowns();
}

void getversion() {
   if (!loadVer()) {
      Serial.println("Failed to load version info");
    } else {
      Serial.println("Version info loaded");
    }
}
bool loadVer() {
  File versionFile = SPIFFS.open("/version.json","r");
  if (!versionFile) {
    Serial.println("Version file not found.");
    return false;
  }
  size_t size = versionFile.size();
  if (size > 512) {
    Serial.println("version file is too large");
    return false;
  }

  // allocate buffer to store contents of the file
  std::unique_ptr<char[]> buf(new char[size]);
  versionFile.readBytes(buf.get(), size);
  Serial.println(buf.get());
  StaticJsonBuffer<512> jsonBufferv;
  JsonObject& vjson = jsonBufferv.parseObject(buf.get());

  if (!vjson.success()) {
    Serial.println("Failed to parse version file");
    return false;
  }
vr.ver = vjson["version"];
//vr.ver = vjson["version"];
//Serial.println(String(vjson["version"]));
//vr.author = vjson["author"];
//vr.company = vjson["company"];
//vr.website = vjson["website"];
//vr.project = vjson["project"];
Serial.print("Version: "); Serial.println(vr.ver);
Serial.print("Author : "); Serial.println(vr.author);
Serial.print("Company: "); Serial.println(vr.company);
Serial.print("Website: "); Serial.println(vr.website);
Serial.print("Project: "); Serial.println(vr.project);

return true;
  
}
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  Serial.print("size ");
  Serial.println(size);
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // allocate buffer to store contents of the file
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  Serial.println(buf.get());
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // const char* ServerName = json["serverName"];
  // and so on to get the values into your constants
  strncpy(HOSTNAME, json["hostname"], 20);
  //HOSTNAME* = json["hostname"];
//  ssid = json["ssid"];
//  char* password =  json["psk"];
//  char* mqtt = json["mqtt"];
  ledtype = json["ledtype"];
  int dp = json["datapin"];
  int cp = json["clockpin"];
  autoeffects = json["autoeffects"];
  randomeffects = json["randomeffects"];
  selectedeffectno = json["selectedeffectno"]; // this is the effect number selected from the web interface or mqtt
  effectselected = json["effectselected"]; // this value will be set to 1 if an effect is selected or chosen from web interface or mqtt
  BRIGHTNESS = json["brightness"];
  FRAMES_PER_SECOND = json["fps"];
  NUM_LEDS = json["numleds"];
  myuniverse = json["universe"];
  
  //NODENAME = json["hostname"]; // i've used NODENAME in the HTML files
  //NUM_LEDS = numleds;
  numberOfChannels = NUM_LEDS * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)

  resetdropdowns();

    
  }

void resetdropdowns() {
    // this bit sets the dropdown box with the last selected effect for manual play
  for (int i=0; i < 20; i ++) {
    if (selectedeffectno == i) { selected[i]="selected"; } else { selected[i]=""; }
    //Serial.println(selected[i]);
  }
  for (int i=0; i<2; i ++) {
    if (ledtype == i) { selectedled[i]="selected"; } else { selectedled[i]=""; }
    if (autoeffects == i) { startupeffectsenabled[i]="selected"; } else { startupeffectsenabled[i]=""; }
    if (randomeffects == i) { cycleeffects[i]="selected"; } else { cycleeffects[i]=""; }
  }
}

void checkConfig() {
  if (!SPIFFS.begin()) {
    Serial.println("Could not see SPIFFS for config file");
    return;
  }
 // if (!saveConfig()) {
 //     Serial.println("Failed to save config");
 //   } else {
 //     Serial.println("Config saved");
//    }
  if (!loadConfig()) {
      Serial.println("Failed to load config");
    } else {
      Serial.println("Config loaded");
    }
}
