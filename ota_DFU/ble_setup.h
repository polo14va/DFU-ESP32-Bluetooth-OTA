#ifndef BLE_SETUP_H
#define BLE_SETUP_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "common_functions.h"
#include "updater_buffers.h"

const char SERVICE_UUID[] = "B200"; // UUID SERVICE
const char CHARACTERISTIC_UUID_RX[] = "fb1e4002-54ae-4a28-9f74-dfccb248601d";// NEED CHOERENCE TO READ SWIFT APP 
const char CHARACTERISTIC_UUID_TX[] = "fb1e4003-54ae-4a28-9f74-dfccb248601d";// NEED CHOERENCE TO READ SWIFT APP 

extern BLECharacteristic* pCharacteristicTX;
extern BLECharacteristic* pCharacteristicRX;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onNotify(BLECharacteristic *pCharacteristic) {
        uint8_t* pData;
        String value = pCharacteristic->getValue();
        int len = value.length();
        pData = pCharacteristic->getData();
        if (pData) {
            Serial.print("TX  ");
            for (int i = 0; i < len; i++) {
                Serial.printf("%02X ", pData[i]);
            }
            Serial.println();
        }
    }

    void onWrite(BLECharacteristic *pCharacteristic) {
        processBLEWrite(pCharacteristic);
    }
};

void initBLE() {
    BLEDevice::init("ESP32 OTA");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    
    pCharacteristicTX = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX, 
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    pCharacteristicRX = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    
    pCharacteristicRX->setCallbacks(new MyCallbacks());
    pCharacteristicTX->addDescriptor(new BLE2902());
    pService->start();
    BLEDevice::startAdvertising();
    Serial.println("OTA Started, you can start this proccess on your device!");
}

#endif
