#ifndef __LS300_V1_PIN_DEFINE_H__
#define __LS300_V1_PIN_DEFINE_H__

// LEDs
#define LED_BUILTIN          (32u) // PC10

#define PIN_BUTTON           (33ul) // PB11
static const uint8_t BUTTON = PIN_BUTTON;

// #define E22_RXEN    // Unused
#define E22_BUSY	  27 // PB0
#define E22_NRST    16 // PA0
#define E22_DIO1    5  // PB3
#define E22_NSS     4  // PA4

#define LS_SKY_ANT 28 // PD0
#define LS_SKY_CSD 29 // PD1
#define LS_SKY_CTX 30 // PC11
#define LS_SKY_CPS 31 // PC12

#define LS_GPS_ENABLE    7  // PB1
#define LS_GPS_V_BCKP    8  // PB8
#define LS_GPS_1PPS      9  // PB9

#define LS_MODULE_ENABLE 7  // PB1
#define LS_SD_ENABLE     26 // PC4

#define LS_BATVOLT_PIN    40 // PC3
#define LS_VERSION_MEAS   39 // PC2
#define LS_VERSION_ENABLE 47 // PC6

#define LS_HALL_ENABLE 24 // PE4
#define LS_HALL_OUTPUT 34 // PC13

#endif /* __LS300_V1_PIN_DEFINE_H__ */
