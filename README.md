# ESP32 OTA Update via BLE/DFU

This repository contains the implementation of an OTA (Over-The-Air) update system for the ESP32 using Bluetooth Low Energy (BLE) with DFU capabilities. The system allows for wireless firmware updates of the ESP32 using a BLE connection, specifically designed to support the [BLEUniversal](https://github.com/polo14va/BLEUniversal) app, a universal Swift application designed to manage and edit BLE characteristics.

## Version

| Version | Description |
| ------- | ----------- |
| 1.0.0   | Initial release with BLE/DFU OTA functionality |

## Getting Started

### Purpose

This project is designed to provide OTA DFU capabilities for ESP32 devices, enabling firmware updates over Bluetooth. It's tailored to support [BLEUniversal](https://github.com/polo14va/BLEUniversal), allowing for seamless firmware updates using devices running macOS (with Apple Silicon Mx series) or iOS (version 17.0 or higher).

### Prerequisites

To run and use this project, you will need:

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- [ESP32 board package](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) for the Arduino IDE
- BLE client app (e.g., nRF Connect, BLE Scanner, or BLEUniversal for DFU over BLE)

### Setup

1. **Install the Arduino IDE and ESP32 Board Package:**
   - Download and install the Arduino IDE from [Arduino Software](https://www.arduino.cc/en/software).
   - Open the Arduino IDE, go to File > Preferences, and add the following URL to the Additional Board Manager URLs:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Open the Board Manager by going to Tools > Board: "..." > Boards Manager..., search for "ESP32", and install the package.

2. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-github-username/esp32-ota-ble.git
   cd esp32-ota-ble
   ```

3. **Open and Upload the Project:**
   - Open the Arduino IDE and then open `ota_process.ino` located in the main project folder.
   - Connect your ESP32 board via USB, select the correct board and port in Tools > Board and Tools > Port.
   - Upload the code by clicking the upload button.

4. **Perform an OTA Update:**
   - Ensure your ESP32 is powered and running the uploaded `ota_process.ino`.
   - Use a BLE client app like BLEUniversal to connect to the ESP32 (named "ESP32 OTA").
   - Follow the OTA update steps in the app using the BLE characteristics defined.

### Usage

To update your ESP32 firmware using DFU over Bluetooth:

1. **Compile the Firmware:**
   - Compile the project using your favorite IDE (this version is designed for Arduino).
   - In Arduino IDE, go to Sketch > Export Compiled Binary to generate the binary file.

2. **Load the Firmware:**
   - Load the binary firmware onto your ESP32 from a Mac (series Mx) or an iPhone/iPad (iOS 17.0 or higher) using BLEUniversal or another BLE DFU app.

## Project Structure

### Files and Their Roles

- **`ota_process.ino`**: The main Arduino sketch that sets up BLE/DFU and handles OTA updates.
- **`ble_setup.h`**: Initializes BLE services and characteristics and defines callbacks for BLE/DFU events.
- **`ota_updates.h`**: Contains functions to handle the OTA update logic, including writing the firmware file and rebooting the ESP32.
- **`common_functions.h`**: Includes utility functions for sending OTA progress and managing BLE/DFU notifications.
- **`updater_buffers.h`**: Defines global variables, buffers, and settings for the OTA process.

### Detailed Functionality

#### `ota_process.ino`

- `setup()`: Initializes serial communication, sets up BLE/DFU services, and prepares the ESP32 for OTA updates.
- `loop()`: Main loop that handles different modes of the OTA process, including normal operation and active updating.

#### `ble_setup.h`

- `initBLE()`: Sets up the BLE server, services, and characteristics for DFU, and starts advertising.
- `MyServerCallbacks`: Handles connect and disconnect events to manage the `deviceConnected` state.
- `MyCallbacks`: Processes incoming data on the RX characteristic and manages notifications on the TX characteristic for DFU progress.

#### `ota_updates.h`

- `writeBinary()`: Writes a chunk of the firmware file to the filesystem for DFU.
- `performUpdate()`: Applies the firmware update from the filesystem to the ESP32.
- `updateFromFS()`: Checks and performs the update if the complete firmware file has been received.
- `handleUpdateMode()`: Manages receiving firmware chunks and updating the DFU progress.
- `handleOtaMode()`: Finalizes the OTA update and reboots the device.

#### `common_functions.h`

- `sendOtaResult()`: Sends the OTA update result via BLE/DFU.
- `sendModeFeedback()`: Sends the current mode and operation feedback to the BLE/DFU client.
- `sendAvailableSpace()`: Sends the available memory space for OTA updates.
- `processBLEWrite()**: Processes incoming write requests on the RX characteristic for DFU.

## Communication Process

| Action           | Sent (TX)               | Received (RX)         | Description                                  |
| ---------------- | ----------------------- | --------------------- | -------------------------------------------- |
| Start OTA        | `0xFD`                  |                       | Indicates the start of the OTA process.      |
| Metadata         |                         | `0xFE [total parts]`  | ESP32 receives the total number of parts.    |
| Chunk Received   |                         | `0xFB [data chunk]`   | ESP32 receives a chunk of firmware data.     |
| Chunk Written    | `Progress: X%`          |                       | ESP32 sends progress after writing a chunk.  |
| Update Complete  | `0xF2`                  |                       | Indicates the end of the update process.     |

## Credits

- **Name**: Pedro Martínez Acebron
- **Website**: [pedromartinezweb.com](https://pedromartinezweb.com)
- **Email**: [hardware@pedromartinezweb.com](mailto:hardware@pedromartinezweb.com)
```

---

### Explanation of Changes and Additions

- **Introduction Updated**: Added the purpose of supporting BLEUniversal and the DFU process.
- **Usage Instructions**: Updated to include steps about compiling the firmware and loading it using devices with macOS or iOS.
- **Detailed Functionality Descriptions**: Included BLE/DFU in descriptions to clarify that the project handles firmware updates using these protocols.
- **Credits**: Provided your full name, website, and email for recognition and further information.
