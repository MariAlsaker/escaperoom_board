#include <ArduinoBLE.h>
#include "ble.h"

BLEService escaperoom_service("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEUnsignedLongCharacteristic board_characteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
// create button characteristic and allow remote device to get notifications
BLEBoolCharacteristic boardreset_characteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLEWrite);

void init_ble(void){
    if (!BLE.begin()) {
        Serial.println("starting Bluetooth® Low Energy module failed!");

        while (1);
    }

    // set the local name peripheral advertises
    BLE.setLocalName("ERC");
    BLE.setManufacturerData(0x01CA, 0, 0);
    // set the UUID for the service this peripheral advertises:
    BLE.setAdvertisedService(escaperoom_service);

    // add the characteristics to the service
    escaperoom_service.addCharacteristic(board_characteristic);
    escaperoom_service.addCharacteristic(boardreset_characteristic);

    // add the service
    BLE.addService(escaperoom_service);

    board_characteristic.writeValue(0xDEADBEEF);
    boardreset_characteristic.writeValue(false);

    // start advertising
    BLE.advertise();

    Serial.println("Bluetooth® device active, waiting for connections...");
}

void set_

void tick_ble(void){
    // poll for Bluetooth® Low Energy events
    BLE.poll();
}