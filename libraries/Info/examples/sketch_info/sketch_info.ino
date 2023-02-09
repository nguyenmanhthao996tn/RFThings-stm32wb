#include "Info.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
}

void loop() {
  delay(2000);
  Info::printSketchInfo(&Serial);
}
