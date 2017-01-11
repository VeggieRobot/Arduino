#include "Wire.h"

int address = 5;

union memory
{
  byte asBytes[4];
  float asFloat;
};
union memory data;

void setup()
{
  Serial.begin(250000);
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
}

void loop()
{
  
}

void receiveEvent(int count)
{
  for(int i = 0; Wire.available(); i++)
  {
    data.asBytes[i] = Wire.read();
  }
  Serial.print(data.asFloat);
}

