#include "Arduino.h"
#include "Wire.h"
#include "TimerMillis.h"
#include "BLE.h"
#include "Info.h"

#define BME280_I2C_ADDRESS 0x76

volatile bool bme280_done = false;

uint8_t bme280_data[8];

TwoWireTransaction bme280_async;

TimerMillis bme280_sample;
TimerMillis bme280_convert;

int16_t temperature;
uint32_t pressure;
uint16_t humidity;

BLEService environmentService("181A");
BLECharacteristic pressureCharacteristic("2A6D", (BLE_PROPERTY_READ | BLE_PROPERTY_NOTIFY), pressure);
BLECharacteristic humidityCharacteristic("2A6F", (BLE_PROPERTY_READ | BLE_PROPERTY_NOTIFY), humidity);
BLECharacteristic temperatureCharacteristic("2A6E", (BLE_PROPERTY_READ | BLE_PROPERTY_NOTIFY), temperature);


void sampleCallback(void)
{
    static uint8_t tx_data[] = { 0xf4, 0x25 };

    bme280_async.submit(Wire, BME280_I2C_ADDRESS, &tx_data[0], 2, NULL, 0, convertCallback);
}

void convertCallback(void)
{
    bme280_convert.start(readyCallback, 10);
}

void readyCallback(void)
{
    static uint8_t tx_data[] = { 0xf7 };

    bme280_async.submit(Wire, BME280_I2C_ADDRESS, &tx_data[0], 1, &bme280_data[0], 8, doneCallback);
}

void doneCallback(void)
{
    int32_t t_fine;
    
    temperature = (int16_t)(bme280_compensate_temperature((((uint32_t)bme280_data[3] << 12) | ((uint32_t)bme280_data[4] << 4) | ((uint32_t)bme280_data[5] >> 4)), t_fine) * 100);
    pressure = (uint32_t)(bme280_compensate_pressure((((uint32_t)bme280_data[0] << 12) | ((uint32_t)bme280_data[1] << 4) | ((uint32_t)bme280_data[2] >> 4)), t_fine) * 10.0);
    humidity = (uint16_t)(bme280_compensate_humidity((((uint32_t)bme280_data[6] << 8) | (uint32_t)bme280_data[7]), t_fine) * 100.0);

    pressureCharacteristic.writeValue(pressure);
    humidityCharacteristic.writeValue(humidity);
    temperatureCharacteristic.writeValue(temperature);

    bme280_done = true;
}

void setup()
{
    Serial.begin(9600);
    
    while (!Serial) { }

    Info::printSketchInfo(&Serial);
    Serial.println();

    Wire.begin();

    bme280_read_calibration_data();
    bme280_write_config();

    bme280_sample.start(sampleCallback, 1000, 2000);

    BLE.begin();
    BLE.setLocalName("RFThings-LS300");

    BLE.setAdvertisedServiceUuid(environmentService.uuid());
    
    environmentService.addCharacteristic(pressureCharacteristic);
    environmentService.addCharacteristic(humidityCharacteristic);
    environmentService.addCharacteristic(temperatureCharacteristic);

    BLE.addService(environmentService);
    
    BLE.advertise();
}

void loop()
{ 
    if (!BLE.advertising() && !BLE.connected()) {
        BLE.advertise();
    }
    
    if (bme280_done) {
        bme280_done = false;
        
        Serial.print("Pressure = ");
        Serial.print((float)pressure / 1000.0);
        Serial.println(" hPa");
        
        Serial.print("Humidity = ");
        Serial.print((float)humidity / 100.0);
        Serial.println(" %");

        Serial.print("Temperature = ");
        Serial.print((float)temperature / 100.0);
        Serial.println(" *C");
        
        Serial.println();
    }
}

/* BME280 Calibration Data */
uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;
uint8_t dig_H1;
int16_t dig_H2;
uint8_t dig_H3;
int16_t dig_H4;
int16_t dig_H5;
int8_t dig_H6;

void bme280_read_calibration_data()
{
    uint8_t data[26];

    Wire.beginTransmission(BME280_I2C_ADDRESS);
    Wire.write(0x88);
    Wire.endTransmission(false);
    Wire.requestFrom(BME280_I2C_ADDRESS, 26);
    Wire.read(&data[0], 26);

    dig_T1 = (uint16_t)(((uint16_t)data[1] << 8) | (uint16_t)data[0]);
    dig_T2 = (int16_t)(((uint16_t)data[3] << 8) | (uint16_t)data[2]);
    dig_T3 = (int16_t)(((uint16_t)data[5] << 8) | (uint16_t)data[4]);
    dig_P1 = (uint16_t)(((uint16_t)data[7] << 8) | (uint16_t)data[6]);
    dig_P2 = (int16_t)(((uint16_t)data[9] << 8) | (uint16_t)data[8]);
    dig_P3 = (int16_t)(((uint16_t)data[11] << 8) | (uint16_t)data[10]);
    dig_P4 = (int16_t)(((uint16_t)data[13] << 8) | (uint16_t)data[12]);
    dig_P5 = (int16_t)(((uint16_t)data[15] << 8) | (uint16_t)data[14]);
    dig_P6 = (int16_t)(((uint16_t)data[17] << 8) | (uint16_t)data[16]);
    dig_P7 = (int16_t)(((uint16_t)data[19] << 8) | (uint16_t)data[18]);
    dig_P8 = (int16_t)(((uint16_t)data[21] << 8) | (uint16_t)data[20]);
    dig_P9 = (int16_t)(((uint16_t)data[23] << 8) | (uint16_t)data[22]);
    dig_H1 = data[25];

    Wire.beginTransmission(BME280_I2C_ADDRESS);
    Wire.write(0xe1);
    Wire.endTransmission(false);
    Wire.requestFrom(BME280_I2C_ADDRESS, 7);
    Wire.read(&data[0], 7);
    
    dig_H2 = (int16_t)(((uint16_t)data[1] << 8) | (uint16_t)data[0]);
    dig_H3 = data[2];
    dig_H4 = (int16_t)((int8_t)data[3] * 16) | (int16_t)(data[4] & 0x0f);
    dig_H5 = (int16_t)((int8_t)data[5] * 16) | (int16_t)(data[4] >> 4);
    dig_H6 = (int8_t)data[6];
}

void bme280_write_config()
{
    Wire.beginTransmission(BME280_I2C_ADDRESS);
    Wire.write(0xf2);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.beginTransmission(BME280_I2C_ADDRESS);
    Wire.write(0xf4);
    Wire.write(0x24);
    Wire.endTransmission();
}

float bme280_compensate_temperature(uint32_t temperature_uncomp, int32_t &t_fine)
{
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 = (int32_t)((temperature_uncomp / 8) - ((int32_t)dig_T1 * 2));
    var1 = (var1 * ((int32_t)dig_T2)) / 2048;
    var2 = (int32_t)((temperature_uncomp / 16) - ((int32_t)dig_T1));
    var2 = (((var2 * var2) / 4096) * ((int32_t)dig_T3)) / 16384;
    t_fine = var1 + var2;
    temperature = (t_fine * 5 + 128) / 256;

    if (temperature < temperature_min) {
        temperature = temperature_min;
    } else if (temperature > temperature_max) {
        temperature = temperature_max;
    }

    return (float)temperature / 100.0f;
}

float bme280_compensate_pressure(uint32_t pressure_uncomp, int32_t t_fine)
{
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int64_t var4;
    uint32_t pressure;
    uint32_t pressure_min = 3000000;
    uint32_t pressure_max = 11000000;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) * 131072);
    var2 = var2 + (((int64_t)dig_P4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t)dig_P3) / 256) + ((var1 * ((int64_t)dig_P2) * 4096));
    var3 = ((int64_t)1) * 140737488355328;
    var1 = (var3 + var1) * ((int64_t)dig_P1) / 8589934592;

    /* To avoid divide by zero exception */
    if (var1 != 0) {
        var4 = 1048576 - pressure_uncomp;
        var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
        var1 = (((int64_t)dig_P9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
        var2 = (((int64_t)dig_P8) * var4) / 524288;
        var4 = ((var4 + var1 + var2) / 256) + (((int64_t)dig_P7) * 16);
        pressure = (uint32_t)(((var4 / 2) * 100) / 128);

        if (pressure < pressure_min) {
            pressure = pressure_min;
        } else if (pressure > pressure_max) {
            pressure = pressure_max;
        }
    } else {
        pressure = pressure_min;
    }

    return (float)pressure / 100.0f;
}

float bme280_compensate_humidity(uint32_t humidity_uncomp, int32_t t_fine)
{
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    uint32_t humidity;
    uint32_t humidity_max = 102400;

    var1 = t_fine - ((int32_t)76800);
    var2 = (int32_t)(humidity_uncomp * 16384);
    var3 = (int32_t)(((int32_t)dig_H4) * 1048576);
    var4 = ((int32_t)dig_H5) * var1;
    var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)dig_H6)) / 1024;
    var3 = (var1 * ((int32_t)dig_H3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)dig_H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)dig_H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max) {
        humidity = humidity_max;
    }

    return (float)humidity / 1024.0f;
}
