#include "Wire.h"

int address = 5;

long curtime = 0;
long lasttime = 0;
long timediff = 0;

union memory
{
  byte asBytes[4];
  float asFloat;
};
union memory data;

void setup()
{
  Serial.begin(250000);
  Wire.begin();
  randomSeed(12345);
}

void loop()
{
//  if(Serial.available() > 0)
//  {
//    data.asFloat = Serial.parseFloat();
//    Serial.println(data.asFloat,10);
//    Wire.beginTransmission(address);
//    Wire.write(data.asBytes,4);
//    Wire.endTransmission();
//  }
  curtime = millis();
  for(int i = 0; i < 1000; i++)
  {
    data.asFloat = 100;//random(1000);
    Wire.beginTransmission(address);
    Wire.write(data.asBytes,4);
    Wire.endTransmission();
  }
  timediff = curtime-lasttime;
  Serial.println(timediff);
  lasttime = curtime;
}
