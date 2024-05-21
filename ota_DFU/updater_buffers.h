#ifndef UPDATER_BUFFERS_H
#define UPDATER_BUFFERS_H

#include <Arduino.h>
#include <SPIFFS.h>

#define FORMAT_SPIFFS_IF_FAILED true
#define FORMAT_FFAT_IF_FAILED true

#define USE_SPIFFS

#ifdef USE_SPIFFS
#define FLASH_SPIFFS SPIFFS
#define FAST_MODE false
#else
#define FLASH_SPIFFS FFat
#define FAST_MODE true
#endif

enum Mode {
  NORMAL_MODE,
  UPDATE_MODE,
  OTA_MODE
};

bool deviceConnected = false, sendMode = false, sendSize = true;
bool writeFile = false, request = false;
bool current = true;
int writeLen = 0, writeLen2 = 0;
int parts = 0, next = 0, cur = 0, MTU = 0;
Mode currentMode = NORMAL_MODE;
unsigned long rParts, tParts;

bool ledState = false;

uint8_t updater[16384];
uint8_t updater2[16384];

void rebootEspWithReason(const String& reason) {
    Serial.println(reason);
    delay(1000);
    ESP.restart();
}

BLECharacteristic* pCharacteristicTX;
BLECharacteristic* pCharacteristicRX;

#endif
