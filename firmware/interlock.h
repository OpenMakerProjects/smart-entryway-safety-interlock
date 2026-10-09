#pragma once
#include <cstdint>
struct Interlock {
 bool armed=false,stable=false,ready=false,valid=false,connected=false;
 int adc=0; uint32_t since=0;
 void tick(uint32_t now,bool inputValid,int light,bool online) {
  adc=light; valid=inputValid&&light>0&&light<4095; connected=online;
  bool safe=valid&&connected&&adc>=500;
  if(!safe){armed=false;stable=false;ready=false;return;}
  if(!stable){stable=true;since=now;}
  ready=uint32_t(now-since)>=3000;
 }
 bool arm(){if(ready&&stable&&valid&&connected&&adc>=500){armed=true;return true;}return false;}
 void stop(){armed=false;stable=false;ready=false;}
 const char* reason()const{return !connected?"offline":!valid?"invalid":adc<500?"dark":!ready?"confirming":"qualified";}
};
