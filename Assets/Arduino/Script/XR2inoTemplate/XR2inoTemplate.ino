/*
 * This script was created by Metagate, an Italian company that develops mixed reality experiences.
 * It is designed to connect an Arduino BLE device to the XR2ino app, developed in Unity, for seamless integration with mixed reality experiences on Meta Quest headsets.
 * 
 * Copyright (c) 2024 Metagate
 * SPDX-License-Identifier: MIT
 * 
 * Feel free to modify and adapt this script to your needs, following our instructions. For a step-by-step tutorial, visit our guide at:
 * [Insert Project Hub link]
 * 
 * We invite you to try our app and share your experiences! 
 * https://vr.meta.me/s/1KVRVO3efRSOYcY
 * 
 * For collaborations or to implement this experience, contact us at:
 * https://discover.themetagate.it/pages/xr2ino
 *
 * This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
 * "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
 * it will remotely display the variables that aren't the default value of "-9".
 */

#include <ArduinoBLE.h> // ArduinoBLE library by Arduino
#include <ArduinoJson.h> // ArduinoJson library by Benoit Blanchon

// Constants for BLE communication
#define MAX_FRAGMENT_SIZE 14  // DO NOT MODIFY: maximum limit of character otherwise the data sent could not be displayed correctly

// Define a custom BLE service and characteristic
BLEService customService("19B10000-E8F2-537E-4F6C-D104768A1214");                                        // DO NOT MODIFY: Custom Service UUID
BLECharacteristic dataCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 512);  // DO NOT MODIFY: Custom Characteristic UUID

// Define the number of sensors
const int NUM_SENSORS = 10; // DO NOT MODIFY: Unity expects precisely 10 variables

// Array to store sensor readings
float variableValues[NUM_SENSORS] = {-9, -9, -9, -9, -9, -9, -9, -9, -9, -9}; // DO NOT MODIFY: Unity expects precisely 10 values

// Names of the sensors. Customize these names based on your project, they will be the titles of each panel in the app. 
const char* variableNames[NUM_SENSORS] = {
  "name01", "name02", "name03", "name04", "name05",
  "name06", "name07", "name08", "name09", "name10"
};

// Timing variables
unsigned long previousMillis = 0;
const long interval = 1000;  // Data sending interval in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Configure BLE device name and advertise the custom service
  BLE.setLocalName("Device_BLE"); // DO NOT MODIFY: the app works only with this local name
  BLE.setAdvertisedService(customService);

  // Add characteristic to the service
  customService.addCharacteristic(dataCharacteristic);

  // Add the service to BLE
  BLE.addService(customService);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  // Wait for BLE central device to connect
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // Sending and reading values only works when connected to the app
    while (central.connected()) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Uncomment lines below to include your sensor readings
        // variableValues[0] = analogRead(A0);
        // variableValues[1] = analogRead(A1);
        // variableValues[2] = analogRead(A2);
        // ...

        // Create a JSON object to hold sensor data
        StaticJsonDocument<512> doc;
        for (int i = 0; i < NUM_SENSORS; i++) {
          doc[variableNames[i]] = variableValues[i];
        }

        // Serialize JSON object into a string
        char jsonBuffer[512];
        size_t len = serializeJson(doc, jsonBuffer);
        Serial.println(jsonBuffer); // Print JSON for debugging

        // Fragment and send JSON data over BLE
        int totalFragments = (len + MAX_FRAGMENT_SIZE - 1) / MAX_FRAGMENT_SIZE;  // Ceiling division
        for (int fragment = 0; fragment < totalFragments; fragment++) {
          int startIdx = fragment * MAX_FRAGMENT_SIZE;
          int endIdx = startIdx + MAX_FRAGMENT_SIZE;
          if (endIdx > len) endIdx = len;

          // Extract a fragment of the JSON string
          char fragmentBuffer[MAX_FRAGMENT_SIZE + 1];
          strncpy(fragmentBuffer, jsonBuffer + startIdx, endIdx - startIdx);
          fragmentBuffer[endIdx - startIdx] = '\0';  // Null-terminate the string

          // Create a header for the fragment and send it
          char sendBuffer[MAX_FRAGMENT_SIZE + 10];
          snprintf(sendBuffer, sizeof(sendBuffer), "%d/%d|%s", fragment + 1, totalFragments, fragmentBuffer);
          Serial.print("Sending fragment: ");
          Serial.println(sendBuffer);

          // Send the fragment over BLE
          bool success = dataCharacteristic.writeValue((uint8_t*)sendBuffer, strlen(sendBuffer));
          if (!success) {
            Serial.println("Failed to send fragment over BLE");
          }

          // Add a small delay between fragments
          delay(100);
        }
      }

      // Handle BLE events
      BLE.poll();
    }

    Serial.println("Disconnected from central");
  }
}
