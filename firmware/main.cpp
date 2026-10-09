#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "interlock.h"
Interlock rule;WiFiClient socketClient;PubSubClient mqtt(socketClient);
Adafruit_SSD1306 oled(128,64,&Wire,-1);bool displayOK=false;
uint32_t lastSample=0,lastReport=0,lastRetry=0;String line;
void command(const String& value){if(value=="ARM")rule.arm();else rule.stop();digitalWrite(25,rule.armed?HIGH:LOW);}
void message(char*,byte* bytes,unsigned int n){String v;for(unsigned i=0;i<n&&i<16;i++)v+=char(bytes[i]);command(n>16?"STOP":v);}
void setup(){
 Serial.begin(115200);pinMode(25,OUTPUT);digitalWrite(25,LOW);
 analogReadResolution(12);analogSetPinAttenuation(34,ADC_11db);pinMode(34,INPUT);
 Wire.begin(21,22);displayOK=oled.begin(SSD1306_SWITCHCAPVCC,0x3c);
 if(WIFI_SSID[0])WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
 mqtt.setServer(MQTT_HOST,1883);mqtt.setCallback(message);mqtt.setBufferSize(512);mqtt.setSocketTimeout(1);mqtt.setKeepAlive(15);
}
void loop(){
 uint32_t now=millis();
 if(WiFi.status()==WL_CONNECTED&&!mqtt.connected()&&MQTT_HOST[0]&&uint32_t(now-lastRetry)>=10000){
  lastRetry=now;
  if(mqtt.connect("entryway-interlock-19",MQTT_USER,MQTT_PASSWORD,"guard19/availability",1,true,"offline")){
   mqtt.publish("guard19/availability","online",true);mqtt.subscribe("guard19/command");
  }
 }
 bool online=WiFi.status()==WL_CONNECTED&&mqtt.connected();
 if(uint32_t(now-lastSample)>=100){lastSample=now;int raw=analogRead(34);rule.tick(now,true,raw,online);digitalWrite(25,rule.armed?HIGH:LOW);}
 if(!online){rule.tick(now,true,rule.adc,false);digitalWrite(25,LOW);}
 if(mqtt.connected())mqtt.loop();
 while(Serial.available()){
  char c=Serial.read();if(c=='\n'){line.trim();command(line);line="";}else if(c!='\r'){line+=c;if(line.length()>16){command("STOP");line="";}}
 }
 if(uint32_t(now-lastReport)>=1000){
  lastReport=now;char json[256];
  snprintf(json,sizeof(json),"{\"id\":19,\"ms\":%lu,\"light_adc\":%d,\"valid\":%s,\"connected\":%s,\"ready\":%s,\"armed\":%s,\"reason\":\"%s\"}",(unsigned long)now,rule.adc,rule.valid?"true":"false",rule.connected?"true":"false",rule.ready?"true":"false",rule.armed?"true":"false",rule.reason());
  Serial.println(json);if(mqtt.connected())mqtt.publish("guard19/state",json,true);
  if(displayOK){oled.clearDisplay();oled.setTextSize(1);oled.setTextColor(SSD1306_WHITE);oled.setCursor(0,0);oled.printf("LDR %d\n%s\n%s",rule.adc,rule.reason(),rule.armed?"PERMISSION ON":"PERMISSION OFF");oled.display();}
 }
}
