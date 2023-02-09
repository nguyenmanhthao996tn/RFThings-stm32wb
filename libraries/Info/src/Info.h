#ifndef __INFO_H__
#define __INFO_H__

#include "Arduino.h"

#define xstr(s) str(s)
#define str(s) #s

class Info
{
public:
  Info(void);
  ~Info(void);

  static void printSketchInfo(HardwareSerial *_serial);

  static const String getSketchName(void);
  static const uint32_t getSketchBuildEpoch(void);
  static const String getSketchBuildOS(void);
  static const uint32_t getBuildIDEVersion(void);

  static const String getBoardVersion(void);
  static const String getCPUSpeed(void);
  static const String getLoRaWANRegion(void);

private:
};

#endif /* __INFO_H__ */
