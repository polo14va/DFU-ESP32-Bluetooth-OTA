#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include "updater_buffers.h"
#include "ble_setup.h"

void sendOtaResult(const String& result) {
    pCharacteristicTX->setValue(result.c_str());
    pCharacteristicTX->notify();
}

void sendModeFeedback() {
    uint8_t fMode[] = {0xAA, FAST_MODE};
    pCharacteristicTX->setValue(fMode, sizeof(fMode));
    pCharacteristicTX->notify();
}

void sendAvailableSpace() {
    unsigned long totalBytes = FLASH_SPIFFS.totalBytes();
    unsigned long usedBytes = FLASH_SPIFFS.usedBytes();
    uint8_t fSize[] = {0xEF, (uint8_t)(totalBytes >> 16), (uint8_t)(totalBytes >> 8), (uint8_t)totalBytes, (uint8_t)(usedBytes >> 16), (uint8_t)(usedBytes >> 8), (uint8_t)usedBytes};
    pCharacteristicTX->setValue(fSize, sizeof(fSize));
    pCharacteristicTX->notify();
}

void processBLEWrite(BLECharacteristic *pCharacteristic) {
    uint8_t* pData;
    String value = pCharacteristic->getValue();
    int len = value.length();
    pData = pCharacteristic->getData();
    if (pData) {
        if (pData[0] == 0xFB) {
            int pos = pData[1];
            for (int x = 0; x < len - 2; x++) {
                if (current) {
                    updater[(pos * MTU) + x] = pData[x + 2];
                } else {
                    updater2[(pos * MTU) + x] = pData[x + 2];
                }
            }
        } else if (pData[0] == 0xFC) {
            if (current) {
                writeLen = (pData[1] * 256) + pData[2];
            } else {
                writeLen2 = (pData[1] * 256) + pData[2];
            }
            current = !current;
            cur = (pData[3] * 256) + pData[4];
            writeFile = true;
            if (cur < parts - 1) {
                request = !FAST_MODE;
            }
        } else if (pData[0] == 0xFD) {
            sendMode = true;
            if (FLASH_SPIFFS.exists("/update.bin")) {
                FLASH_SPIFFS.remove("/update.bin");
            }
        } else if (pData[0] == 0xFE) {
            rParts = 0;
            tParts = (pData[1] * 256 * 256 * 256) + (pData[2] * 256 * 256) + (pData[3] * 256) + pData[4];
            parts = (pData[5] * 256) + pData[6];
            Serial.print("Total parts: ");
            Serial.println(parts);
            Serial.print("File Size: ");
            Serial.println(tParts);
        } else if (pData[0] == 0xFF) {
            parts = (pData[1] * 256) + pData[2];
            MTU = (pData[3] * 256) + pData[4];
            currentMode = UPDATE_MODE;
        } else if (pData[0] == 0xEF) {
            FLASH_SPIFFS.format();
            sendSize = true;
        }
    }
}


#endif
