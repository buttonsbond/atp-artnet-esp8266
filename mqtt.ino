char effecttopic[40];
char startuptopic[40];
char cycletopic[40];
char offtopic[40];
char value[5];

#ifdef ENABLE_MQTT

// char mqtt_intopic[strlen(HOSTNAME) + 4 + 5];      // Topic in will be: <HOSTNAME>/in
// char mqtt_outtopic[strlen(HOSTNAME) + 5 + 5];     // Topic out will be: <HOSTNAME>/out
#define mqtt_server "192.168.1.131"
//strncpy(MQTTHOST,HOSTNAME,20);
#define mqtt_user "art1"
#define mqtt_password "art1"
#define mqtt_port 1883




#endif // of ifdef enable_mqtt

void mqtt_setup(){

client.setServer(mqtt_server, mqtt_port); // this established connection
client.setCallback(mqttcallback); // this tells where to send us if we get a message
sprintf(effecttopic, "%s/%s", HOSTNAME, "effect");
sprintf(startuptopic, "%s/%s", HOSTNAME, "startup");
sprintf(cycletopic, "%s/%s", HOSTNAME, "cycle");
sprintf(offtopic, "%s/%s", HOSTNAME, "off");
mqttconnect();
mqttpublish();
mqttsubscribe();
}

void mqttconnect(){
while (!client.connected()) {
Serial.println("Connecting to MQTT...");
client.connect(HOSTNAME, mqtt_user, mqtt_password);
if (client.connect(HOSTNAME, mqtt_user, mqtt_password )) {
Serial.println("mqtt connected");
}
} 
}

void mqttpublish(){
  String tt = String(selectedeffectno);
  sprintf(value,"%u",selectedeffectno);
  client.publish(effecttopic,value,false);
  client.publish(startuptopic,(autoeffects ==1)? "1": "0",false);
  client.publish(cycletopic,(randomeffects == 1)? "1": "0",false);
  client.publish(offtopic,(autoeffects ==1)? "1": "0",false);
}
void mqttsubscribe(){
  client.subscribe(effecttopic);
  client.subscribe(startuptopic);
  client.subscribe(cycletopic);
  client.subscribe(offtopic); 
}

void mqttcallback(String topic, byte* payload, unsigned int length) {
  String msg = "";
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("payload length was: ");
  Serial.println(length);
  payload[length] = '\0';
  msg = (char*)payload;
  Serial.print("Message:");
  Serial.println(msg);
 if (topic == String(effecttopic)) {
    Serial.print("mqtt effect message received: "); 
    Serial.println(msg);
    selectedeffectno=msg.toInt();
    savesettings();
    }
 if (topic == String(startuptopic)) { 
    Serial.print("mqtt startup message received: ");
    Serial.println(msg); 
    if (msg == "1") { autoeffects=1; } else { autoeffects=0; } 
    savesettings(); 
  }
 if (topic == String(cycletopic)) {
    Serial.print("Cycle topic message received: ");
    Serial.println(msg); 
    if (msg == "1") { randomeffects = 1; } else { randomeffects=0; }
    savesettings();
  }
 if (topic == String(offtopic)) {
  Serial.print("turn off topic received: ");
  autoeffects=0;
  savesettings();
  lightsoff();
  Serial.println(msg);
 }
}
