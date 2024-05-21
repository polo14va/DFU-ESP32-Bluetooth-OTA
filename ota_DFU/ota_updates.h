#ifndef OTA_UPDATES_H
#define OTA_UPDATES_H

#include <Update.h>
#include "common_functions.h"
#include "updater_buffers.h"

void writeBinary(const char * path, uint8_t *data, int len) {
    File file = FLASH_SPIFFS.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("- failed to open file for writing");
        return;
    }
    file.write(data, len);
    file.close();
    writeFile = false;
    rParts += len;
}

void performUpdate(Stream &updateSource, size_t updateSize) {
    String result = String(0x0F);
    if (Update.begin(updateSize)) {
        size_t written = Update.writeStream(updateSource);
        result += "Written : " + String(written) + "/" + String(updateSize) + " [" + String((written / updateSize) * 100) + "%] \n";
        if (Update.end()) {
            result += "OTA Done: ";
            if (Update.isFinished()) {
                result += "Success!\n";
            } else {
                result += "Failed!\n";
            }
        } else {
            result += "Error #: " + String(Update.getError());
        }
    } else {
        result += "Not enough space for OTA";
    }
    sendOtaResult(result);
}

void updateFromFS() {
    File updateBin = FLASH_SPIFFS.open("/update.bin");
    if (updateBin && !updateBin.isDirectory()) {
        size_t updateSize = updateBin.size();
        if (updateSize > 0) {
            performUpdate(updateBin, updateSize);
        } else {
            Serial.println("Error, file is empty");
        }
        updateBin.close();
        FLASH_SPIFFS.remove("/update.bin");
        rebootEspWithReason("Rebooting to complete OTA update");
    } else {
        Serial.println("Could not load update.bin");
    }
}

void handleUpdateMode() {
    if (request) {
        uint8_t rq[] = {0xF1, (uint8_t)((cur + 1) / 256), (uint8_t)((cur + 1) % 256)};
        pCharacteristicTX->setValue(rq, sizeof(rq));
        pCharacteristicTX->notify();
        request = false;
    }

    if (cur + 1 == parts) {
        uint8_t com[] = {0xF2, (uint8_t)((cur + 1) / 256), (uint8_t)((cur + 1) % 256)};
        pCharacteristicTX->setValue(com, sizeof(com));
        pCharacteristicTX->notify();
        currentMode = OTA_MODE;
    }

    if (writeFile) {
        if (!current) {
            writeBinary("/update.bin", updater, writeLen);
        } else {
            writeBinary("/update.bin", updater2, writeLen2);
        }
        // Calculate and print the progress
        float progress = (float)(cur + 1) / parts * 100.0f;
        Serial.print("Progress: ");
        Serial.print(progress, 2);
        Serial.println("%");
    }
}


void handleOtaMode() {
    if (writeFile) {
        if (!current) {
            writeBinary("/update.bin", updater, writeLen);
        } else {
            writeBinary("/update.bin", updater2, writeLen2);
        }
    }

    if (rParts == tParts) {
        Serial.println("Complete");
        delay(5000);
        updateFromFS();
    } else {
        Serial.println("Incomplete");
        Serial.print("Expected: ");
        Serial.print(tParts);
        Serial.print("Received: ");
        Serial.println(rParts);
        delay(2000);
    }
}

#endif
