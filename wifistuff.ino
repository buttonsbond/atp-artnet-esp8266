
void ondemandsetup() {
  pinMode(TRIGGER_PIN, INPUT);
}


String scanwifi() {
  String json = "[";
  int n = WiFi.scanComplete();
  if(n == -2){
    WiFi.scanNetworks(true);
  } else if(n){
    for (int i = 0; i < n; ++i){
      if(i) json += ",";
      json += "{";
      json += "\"rssi\":"+String(WiFi.RSSI(i));
      json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
      json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
      json += ",\"channel\":"+String(WiFi.channel(i));
      json += ",\"secure\":"+String(WiFi.encryptionType(i));
      json += ",\"hidden\":"+String(WiFi.isHidden(i)?"true":"false");
      json += "}";
    }
    WiFi.scanDelete();
    if(WiFi.scanComplete() == -2){
      WiFi.scanNetworks(true);
    }
  }
  json += "]";
  return json;
  //json = String();
}

void resetwifi() {
    AsyncWiFiManager wifiManager(&serverap,&dns);
    Serial.println("WiFi Config Portal Requested");
    Serial.println("You may need to connect to the controllers access point at http://192.168.4.1 to change your WiFi");
    //wifiManager.resetSettings();
    wifiManager.startConfigPortal("OnDemandAP");
    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      //ESP.reset();
      delay(5000);
    }
}

void checkwifi() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW ) {

  }

    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setTimeout(120);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    
   

    WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
      Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());

//   while (WiFi.waitForConnectResult() != WL_CONNECTED) {
 //   Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
//    ESP.restart();
 // }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
