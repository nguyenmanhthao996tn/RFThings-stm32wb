#include "Info.h"

Info::Info(void)
{
}

Info::~Info(void)
{
}

void Info::printSketchInfo(HardwareSerial *_serial)
{
  if (_serial != NULL)
  {
    _serial->println("=== Source Code Information ===");

    _serial->print("   Filename: ");
    _serial->println(Info::getSketchName());

    _serial->print("   Build EPOCH time: ");
    _serial->println(Info::getSketchBuildEpoch());

    _serial->print("   Build OS: ");
    _serial->println(Info::getSketchBuildOS());

    _serial->print("   IDE Version: ");
    _serial->println(Info::getBuildIDEVersion());

    _serial->print("   Board version: ");
    _serial->println(Info::getBoardVersion());

    _serial->print("   CPU Speed: ");
    _serial->println(Info::getCPUSpeed());

    _serial->print("   LoRaWAN Region: ");
    _serial->println(Info::getLoRaWANRegion());
  }
}

const String Info::getSketchName(void)
{
#if defined(__FILENAME__)
  return (__FILENAME__);
#else
  return "";
#endif
}

const uint32_t Info::getSketchBuildEpoch(void)
{
#if defined(__BUILD_TIME__)
  return (__BUILD_TIME__);
#else
  return 0;
#endif
}

const String Info::getSketchBuildOS(void)
{
#if defined(__BUILD_OS__)
  return (__BUILD_OS__);
#else
  return "";
#endif
}

const uint32_t Info::getBuildIDEVersion(void)
{
#if defined(__IDE_VERSION__)
  return (__IDE_VERSION__);
#else
  return 0;
#endif
}

const String Info::getBoardVersion(void)
{
#if defined(BOARD_VERSION)
  switch (BOARD_VERSION)
  {
  case BOARD_LS200_V2:
    return "BOARD_LS200_V2";
    break;
  case BOARD_LS200_V3:
    return "BOARD_LS200_V3";
    break;
  case BOARD_LS200_V4:
    return "BOARD_LS200_V4";
    break;
  case BOARD_LS200_V5:
    return "BOARD_LS200_V5";
    break;
  case BOARD_LS300_V1:
    return "BOARD_LS300_V1";
    break;
  case BOARD_LS300_V0:
    return "BOARD_LS300_V0";
    break;
  default:
    return "Not defined";
    break;
  }
#else
  return "Not defined";
#endif
}

const String Info::getCPUSpeed(void)
{
#if defined(__SYSTEM_CORE_CLOCK__)
  return xstr(__SYSTEM_CORE_CLOCK__);
#else
  return "Not defined";
#endif
}

const String Info::getLoRaWANRegion(void)
{
#if defined(REGION)
  return xstr(REGION);
#else
  return "Not defined";
#endif
}
