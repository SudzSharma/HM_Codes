#include "Wire.h"
#define PCF85911 (0x90 >> 1) // I2C bus address
int y;  //int
#define min 00  //minimum value of input
#define max 256 //maximum value of input
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //char x;
}
void dac_data(int data)
{
  Wire.beginTransmission(PCF85911);
  Wire.write(0x40);
  Wire.write(data);
  Wire.endTransmission();
}
void loop()
{
  y =;//write an input in this statement for y value
  y = map(y, min, max, 0, 256);
  if (y >= 0)
  {
    dac_data(y);

  }
}
