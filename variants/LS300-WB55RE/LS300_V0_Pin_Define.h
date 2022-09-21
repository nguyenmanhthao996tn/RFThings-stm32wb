#ifndef __LS300_V0_PIN_DEFINE_H__
#define __LS300_V0_PIN_DEFINE_H__

// LEDs
#define LED_BUILTIN          (2u) // PB5

#define PIN_BUTTON           (26ul) // PC4
static const uint8_t BUTTON = PIN_BUTTON;

// #define E22_RXEN    // Unused
#define E22_BUSY	  27 // PB0
#define E22_NRST    16 // PA0
#define E22_DIO1    3  // PB4
#define E22_NSS     4  // PA4

#define LS_SKY_ANT 28 // PD0
#define LS_SKY_CSD 29 // PD1
#define LS_SKY_CTX 30 // PC11
#define LS_SKY_CPS 31 // PC12

#define LS_GPS_ENABLE    LS_MODULE_ENABLE
#define LS_GPS_V_BCKP    24  // PE4
#define LS_GPS_1PPS      32  // PC10

#define LS_MODULE_ENABLE 7  // PB1
#define LS_SD_ENABLE     9 // PB9

#define LS_BATVOLT_PIN    40 // PC3
#define LS_VERSION_MEAS   38 // PC1
#define LS_VERSION_ENABLE 47 // PC6

#define LS_HALL_ENABLE 25 // PB3
#define LS_HALL_OUTPUT 17 // PA1

#endif /* __LS300_V0_PIN_DEFINE_H__ */
