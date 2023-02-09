#include "Info.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
  }
}

void loop()
{
  delay(2000);

  Serial.println("=== Source Code Information ===");

  Serial.print("   Filename: ");
  Serial.println(Info::getSketchName());

  Serial.print("   Build EPOCH time: ");
  Serial.println(Info::getSketchBuildEpoch());

  Serial.print("   Build OS: ");
  Serial.println(Info::getSketchBuildOS());

  Serial.print("   IDE Version: ");
  Serial.println(Info::getBuildIDEVersion());

  Serial.print("   Board version: ");
  Serial.println(Info::getBoardVersion());

  Serial.print("   CPU Speed: ");
  Serial.println(Info::getCPUSpeed());

  Serial.print("   LoRaWAN Region: ");
  Serial.println(Info::getLoRaWANRegion());
}
