#include "ble_setup.h"
#include "ota_updates.h"
#include "common_functions.h"
#include "updater_buffers.h"

#define FIRMWARE_VERSION "1.0.0"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32 OTA Update via BLE/DFU");
  Serial.println("Firmware Version: " + String(FIRMWARE_VERSION));
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  if (!FLASH_SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  initBLE();
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
}

void handleNormalMode() {
  if (deviceConnected) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (sendMode) {
      sendModeFeedback();
      sendMode = false;
    }

    if (sendSize) {
      sendAvailableSpace();
      sendSize = false;
    }
    toggleLED();
    delay(100);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  switch (currentMode) {
    case NORMAL_MODE:
      handleNormalMode();
      break;
    case UPDATE_MODE:
      handleUpdateMode();
      break;
    case OTA_MODE:
      handleOtaMode();
      break;
  }
}
