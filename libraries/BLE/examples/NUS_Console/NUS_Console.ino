#include "Arduino.h"
#include "STM32WB.h"
#include "BLE.h"
#include "Info.h"

BLEUart SerialBLE(BLE_UART_PROTOCOL_NORDIC);

void setup()
{
    Serial.begin(9600);
    
    while (!Serial) { }

    Info::printSketchInfo(&Serial);
    Serial.println();

    BLE.begin();
    BLE.setLocalName("RFThings-LS300");
    BLE.setAdvertisedServiceUuid(SerialBLE.uuid());

    BLE.addService(SerialBLE);
    
    BLE.advertise();

    delay(1000);
}

void loop()
{ 
    int c;
    
    if (!BLE.advertising() && !BLE.connected()) {
        BLE.advertise();
    }

    SerialBLE.print("Temperature = ");
    SerialBLE.print(STM32WB.readTemperature());
    SerialBLE.println(" *C");

    SerialBLE.print("Battery = ");
    SerialBLE.print(STM32WB.readBattery());
    SerialBLE.println(" V");

    SerialBLE.println();

    while ((c = SerialBLE.read()) >= 0) { if (c == '\n') { Serial.write('\r'); } Serial.write(c); }
    
    delay(2000);
}
