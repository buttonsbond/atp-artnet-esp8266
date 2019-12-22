   void wemosetup() {
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    fauxmo.enable(true);
    // Add virtual devices
    fauxmo.addDevice(HOSTNAME);
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        
          Serial.println("Received voice command.");
          if (!state) {
            Serial.println("...turn off");
            autoeffects=0;
          }
          if (state) {
            Serial.println("...turn on");
            autoeffects=1;
          }
          // percentage values to map to effectno
          // there are currently 11 built in effects dec 2019 - I've catered here for another 4
          if (value == 4) { selectedeffectno=0; }
          if (value == 6) { selectedeffectno=1; }
          if (value == 9) { selectedeffectno=2; }
          if (value == 11) { selectedeffectno=3; }
          if (value == 14) { selectedeffectno=4; }
          if (value == 16) { selectedeffectno=5; }
          if (value == 19) { selectedeffectno=6; }
          if (value == 21) { selectedeffectno=7; }
          if (value == 24) { selectedeffectno=8; }
          if (value == 26) { selectedeffectno=9; }
          if (value == 29) { selectedeffectno=10; }
          if (value == 31) { selectedeffectno=11; }
          if (value == 34) { selectedeffectno=12; }
          if (value == 36) { selectedeffectno=13; }
          if (value == 39) { selectedeffectno=14; }

          savesettings();  

           if (!state) { lightsoff(); }

    });

}
