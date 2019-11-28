

// IPAddress dns(8, 8, 8, 8);


void savesettings() {
  Serial.println("attempting save...");
  saveConfiguration();
  delay(pause);
  }

String processor(const String& var){
//Serial.println(var);  
if (var == "CURRENTIP") {
      return (IPtoString(WiFi.localIP()));
      }
if (var == "CURRENTWIFI") {
      return (WiFi.SSID());
      }
if (var == "CURRENTUNIVERSE") {
      return String(myuniverse);
      }
if (var == "CURRENTNODES") {
      return String(NUM_LEDS);
      }
if (var == "CURRENTCHANNELS") {
      return String(numberOfChannels);
      }
if (var == "NODENAME") {
      return String(HOSTNAME);
      }
// this is reponsible for sticking the selected LED type on the drop down
if (var == "selectedled[0]") { return selectedled[0]; Serial.println("led set 0 selected"); }
if (var == "selectedled[1]") { return selectedled[1]; Serial.println("led set 1 selected"); }
if (var == "startupeffects[0]") { return startupeffectsenabled[0]; }
if (var == "startupeffects[1]") { return startupeffectsenabled[1]; }
if (var == "cycleeffects[0]") { return cycleeffects[0]; }
if (var == "cycleeffects[1]") { return cycleeffects[1]; }

if (var == "selected[0]") {
      return selected[0];
      }
if (var == "selected[1]") {
      return selected[1];
      }
if (var == "selected[2]") {
      return selected[2];
      }
if (var == "selected[3]") {
      return selected[3];
      }
if (var == "selected[4]") {
      return selected[4];
      }
if (var == "selected[5]") {
      return selected[5];
      }
if (var == "selected[6]") {
      return selected[6];
      }
if (var == "selected[7]") {
      return selected[7];
      }
if (var == "selected[8]") {
      return selected[8];
      }
if (var == "selected[9]") {
      return selected[9];
      }
if (var == "selected[10]") {
      return selected[10];
      }
//if (var == "startupeffects") {
//      return String(startupeffects());
//}
if (var == "dp") { return String(DATA_PIN);  } // return dp 
if (var == "cp") { return String(CLOCK_PIN); } // return cp 

}

//String startupeffects() {
//  if (autoeffects == 1) { return "checked"; } else { return ""; }
//}
String redirect() {
  String rd = "http://";
  String rd2 = ".local/";
  return (rd + String(HOSTNAME) + rd2);
}

String IPtoString(const IPAddress& address){
  return String() + address[0] + "." + address[1] + "." + address[2] + "." + address[3];
}
 
void websetup(){
  //Serial.begin(115200);

// Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  //dns.start(53, "*", WiFi.localIP());
 
  server.on("/rand", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(1000)));
  });
  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = scanwifi();
    request->send(200, "application/json", json);
  });
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
   // Route for css sheets
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  // routes for config pages
  server.on("/led.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/led.html", String(), false, processor);
  });
  server.on("/builtin.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/builtin.html", String(), false, processor);
  });
  server.on("/network.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/network.html", String(), false, processor);
  });  
  server.on("/reboot.html", HTTP_GET, [](AsyncWebServerRequest *request){
    //call save-before-reboot;
    //request->send(SPIFFS, "/reboot.html", String(), false, processor);
    request->redirect(redirect());
    delay(pause);   
    reboot();
  });  
  server.on("/resetwifi.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect(redirect());
    delay(pause);
    resetwifi();   
    //reboot();
  });  

  
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery.min.js", "text/javascript");
  });
   server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.js", "text/javascript");
  });
     server.on("/scripts.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/scripts.js", "text/javascript");
  });
  // process submit form
     server.on("/save.html", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("Checking which parameter to save...");
          int params = request->params();
          Serial.printf("%d params sent in\n", params); Serial.println("");
          for (int i = 0; i < params; i++)
          {
              AsyncWebParameter *p = request->getParam(i);
              if (p->isFile())
              {
                  Serial.printf("_FILE[%s]: %s, size: %u", p->name().c_str(), p->value().c_str(), p->size());
              }
              else if (p->isPost())
              {
                  Serial.printf("_POST[%s]: %s", p->name().c_str(), p->value().c_str());
              }
              else
              {
                  Serial.printf("_GET[%s]: %s", p->name().c_str(), p->value().c_str());
              }
              Serial.println("");
          }
          // 
         if (request->hasParam("effect"))
            {
                Serial.printf("Starting effect %s",request->getParam("effect")->value().c_str());
                Serial.println("");
                selectedeffectno = request->getParam("effect")->value().toInt();
                //effectselected=1;
                // code to start the effect
            }
         // changing the pixel string type
         if (request->hasParam("curtype")) { ledtype = request->getParam("curtype")->value().toInt(); }
         // startupeffects can be changed from on or off 1 or 0 to determine if on startup
         // either the chosen single effect is started or cycling through all builtin effects
         if (request->hasParam("startupeffects"))
            {
              // see what value we have for startup effects on or null
              int s = request->getParam("startupeffects")->value().toInt();
              if (s == 1) { autoeffects=1; } else { autoeffects=0; }
            }
         // cater for changing value of cycling effects on startup or the one selected effect
         if (request->hasParam("cycleeffects")) {
              int s = request->getParam("cycleeffects")->value().toInt();
              if (s == 1) { randomeffects = 1; } else { randomeffects=0; }
         }
         if (request->hasParam("curuniverse"))
            {
                myuniverse = request->getParam("curuniverse")->value().toInt();
            }
         if (request->hasParam("curnodes"))
            {
                NUM_LEDS = request->getParam("curnodes")->value().toInt();
                numberOfChannels = NUM_LEDS * 3;
            }
            if (request->hasParam("curip"))
            {
              IPAddress ip;
                if (ip.fromString(String(request->getParam("curip")->value()))) {   // str = char * or String
                     // it is a valid IP address
                     ip = ip.fromString(String(request->getParam("curip")->value()));
                     //WiFi.config(ip,dns,WiFi.gatewayIP(),WiFi.subnetMask());
                     //WiFi.begin();
                     //WiFi.config(ip);
                      }
                //myuniverse = request->getParam("curuniverse", true)->value();
            }
   // check if nodename has been changed
   if (request->hasParam("NODENAME")) { strncpy(HOSTNAME,request->getParam("NODENAME")->value().c_str(),20); }
   
                //myuniverse = request->getParam("curuniverse", true)->value();
          // do any other saving - ie making changes permanent
        //request->send(SPIFFS, "/index.html", "text/html");
        savesettings();
          //ESP.restart();
        delay(pause);
    request->redirect(redirect());
        delay(pause);
        reboot();
        //delay(3000);
        //Serial.begin(57600); // i think I read that reini serial will restart the sketch?
          //request->send(SPIFFS, "/index.html", String(), false, processor);
 
          
      });

  server.begin();
}
 
void webloop(){

  }
