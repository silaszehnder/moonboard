/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

/* #include <Arduino.h> */
/* #include <BLEDevice.h> */
/* #include <BLEUtils.h> */
/* #include <BLEServer.h> */

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

/* void setup() { */
/*   Serial.begin(115200); */
/*   delay(5000); */
/*   Serial.println("Starting BLE work!"); */

/*   if (!BLEDevice::init("BLE Server Example")) { */
/*     Serial.println("BLE initialization failed!"); */
/*     return; */
/*   } */

/*   BLEServer *pServer = BLEDevice::createServer(); */
/*   BLEService *pService = pServer->createService(SERVICE_UUID); */
/*   pServer->advertiseOnDisconnect(true); */
/*   BLECharacteristic *pCharacteristic = */
/*     pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE); */

/*   pCharacteristic->setValue("Hello World says Neil"); */
/*   pService->start(); */
/*   // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility */
/*   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); */
/*   pAdvertising->addServiceUUID(SERVICE_UUID); */
/*   pAdvertising->setScanResponse(true); */
/*   pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue */
/*   pAdvertising->setMaxPreferred(0x12); */
/*   BLEDevice::startAdvertising(); */
/*   Serial.println("Characteristic defined! Now you can read it in your phone!"); */
/* } */

/* void loop() { */
/*   // put your main code here, to run repeatedly: */
/*   delay(2000); */
/*   Serial.println("hi"); */
/* } */
/* #include <Arduino.h> */
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <NeoPixelBus.h>
#include <config.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      Serial.println();
      Serial.println("*********");
    }
  }
};

/* #ifdef GRB */
/* NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin); */
/* #else */
/* NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin); */
/* #endif */

/* RgbColor red(BRIGHTNESS, 0, 0); */
/* RgbColor green(0, BRIGHTNESS, 0); */
/* RgbColor blue(0, 0, BRIGHTNESS); */
/* RgbColor yellow(ADDITIONAL_LED_BRIGHTNESS, ADDITIONAL_LED_BRIGHTNESS, 0); */
/* RgbColor cyan(0, BRIGHTNESS, BRIGHTNESS); */
/* RgbColor pink(BRIGHTNESS, 0, BRIGHTNESS/2); */
/* RgbColor violet(BRIGHTNESS/2, 0, BRIGHTNESS); */
/* RgbColor black(0); */

/* int state = 0; // Variable to store the current state of the problem string parser */
/* String problemstring = ""; // Variable to store the current problem string */
/* bool useadditionalled = false; // Variable to store the additional LED setting */
/* int problem_loaded_time = 0; // Variable to store the time when a problem was loaded */

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing BLEDevice");

  // Create the BLE Device
  BLEDevice::init("MoonBoard");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  // Descriptor 2902 is not required when using NimBLE as it is automatically added based on the characteristic properties
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->addServiceUUID(pService->getUUID());
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

/*   strip.Begin(); // Initialize LED strip */
/*   strip.Show(); // Good practice to call Show() in order to clear all LEDs */

/*   // Test LEDs by cycling through the colors and then turning the LEDs off again */
/*   strip.SetPixelColor(0, green); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, blue); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, yellow); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, cyan); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, pink); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, violet); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */
/*   strip.SetPixelColor(0, red); */
/*   for (int i = 0; i < PixelCount; i++) { */
/*     strip.ShiftRight(1); */
/*     strip.Show(); */
/*     delay(10); */
/*   } */

/*   strip.ClearTo(black); */
/*   strip.Show(); */
}

void loop() {
  if (deviceConnected) {
    /* Serial.print("Notifying Value: "); */
    /* Serial.println(txValue); */
    /* pTxCharacteristic->setValue(&txValue, 1); */
    /* pTxCharacteristic->notify(); */
    txValue++;
    delay(1000);  // Notifying every 1 second
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("Started advertising again...");
    oldDeviceConnected = false;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = true;
  }
  String stackType = BLEDevice::getBLEStackString();

/*   /1* // Read messages from the BLE UART *1/ */
/*   /1* bleSerial.poll(); *1/ */
  
/*   /1* // Check if message parts are available on the BLE UART *1/ */
/*   /1* while (bleSerial.available() > 0) { *1/ */
/*   /1*   char c = bleSerial.read(); *1/ */

/*   /1*   // State 0: wait for configuration instructions (sent only if V2 option is enabled in app) or beginning of problem string *1/ */
/*   /1*   if (state == 0) { *1/ */
/*   /1*     if (c == '~') { *1/ */
/*   /1*       state = 1; // Switch to state 1 (read configuration) *1/ */
/*   /1*       continue; *1/ */
/*   /1*     } *1/ */
/*   /1*     if (c == 'l') { *1/ */
/*   /1*       state = 2; // Switch to state 2 (read problem string) *1/ */
/*   /1*       continue; *1/ */
/*   /1*     } *1/ */
/*   /1*   } *1/ */

/*   /1*   // State 1: read configuration option (sent only if V2 option is enabled in app) *1/ */
/*   /1*   if (state == 1) { *1/ */ 
/*   /1*     if (c == 'D') { *1/ */
/*   /1*       useadditionalled = true; *1/ */
/*   /1*       state = 2; // Switch to state 2 (read problem string) *1/ */
/*   /1*       continue; *1/ */
/*   /1*     } *1/ */
/*   /1*     if (c == 'l') { *1/ */
/*   /1*       state = 2; // Switch to state 2 (read problem string) *1/ */
/*   /1*       continue; *1/ */        
/*   /1*     } *1/ */
/*   /1*   } *1/ */

/*   /1*   // State 2: wait for the second part of the beginning of a new problem string (# after the lower-case L) *1/ */
/*   /1*   if (state == 2) { *1/ */
/*   /1*     if (c == '#') { *1/ */
/*   /1*       state = 3; // Switch to state 3 *1/ */
/*   /1*       continue; *1/ */
/*   /1*     } *1/ */
/*   /1*   } *1/ */

/*   /1*   // State 3: store hold descriptions in problem string *1/ */
/*   /1*   if (state == 3) { *1/ */ 
/*   /1*     if (c == '#') { // problem string ends with # *1/ */
/*   /1*       state = 4; // Switch to state 4 (start parsing and show LEDs) *1/ */
/*   /1*       continue; *1/ */
/*   /1*     } *1/ */
/*   /1*     problemstring.concat(c); // add current character to problem string *1/ */
/*   /1*   } *1/ */
/*   /1* } *1/ */

/*   /1* // State 4: complete problem string received, start parsing *1/ */
/*   /1* if (state == 4) { *1/ */
/*   /1*   strip.ClearTo(black); // Turn off all LEDs in LED string *1/ */
/*   /1*   Serial.println("\n---------"); *1/ */
/*   /1*   Serial.print("Problem string: "); *1/ */
/*   /1*   Serial.println(problemstring); *1/ */
/*   /1*   Serial.println(""); *1/ */

/*   /1*   String problemstringstore = problemstring; // store copy of problem string *1/ */

/*   /1*   if (useadditionalled) { // only render additional LEDs in first loop *1/ */
/*   /1*     Serial.println("Additional LEDs:"); *1/ */
/*   /1*     while(true){ *1/ */
/*   /1*       int pos = problemstring.indexOf(','); // Hold descriptions are separated by a comma (,) *1/ */
        
/*   /1*       String hold; *1/ */
/*   /1*       if (pos > 0) { // Still holds left in the problem string *1/ */
/*   /1*         hold = problemstring.substring(0, pos); // Extract one hold description *1/ */
/*   /1*       } *1/ */
/*   /1*       else { // Last hold in the problem string *1/ */
/*   /1*         hold = problemstring; *1/ */ 
/*   /1*       } *1/ */

/*   /1*       char holdtype = hold.charAt(0); // Hold descriptions consist of a hold type (S, P, E) ... *1/ */
/*   /1*       int holdnumber = hold.substring(1).toInt(); // ... and a hold number *1/ */
/*   /1*       int lednumber = ledmapping[holdnumber]; *1/ */
/*   /1*       int additionallednumber = lednumber + additionalledmapping[holdnumber]; *1/ */
/*   /1*       if (additionalledmapping[holdnumber] != 0) { *1/ */
/*   /1*         Serial.print(holdtype); *1/ */
/*   /1*         Serial.print(holdnumber); *1/ */
/*   /1*         Serial.print(" --> "); *1/ */
/*   /1*         Serial.print(additionallednumber); *1/ */
/*   /1*         if (holdtype == 'S') { // Start hold *1/ */
/*   /1*           strip.SetPixelColor(additionallednumber, yellow); *1/ */
/*   /1*           Serial.println(" (yellow)"); *1/ */
/*   /1*         } *1/ */
/*   /1*         // Right, left, match, or foot hold *1/ */
/*   /1*         if (holdtype == 'R' || holdtype == 'L' || holdtype == 'M' || holdtype =='F' || holdtype =='P') { *1/ */
/*   /1*           strip.SetPixelColor(additionallednumber, yellow); *1/ */
/*   /1*           Serial.println(" (yellow)"); *1/ */
/*   /1*         } *1/ */
/*   /1*         // Finish holds don't get an additional LED! *1/ */
/*   /1*       } *1/ */        

/*   /1*       if (pos == -1) { // Last hold has been processed! *1/ */
/*   /1*         Serial.println(""); *1/ */
/*   /1*         break; *1/ */
/*   /1*       } *1/ */

/*   /1*       problemstring = problemstring.substring(pos+1, problemstring.length()); // Remove processed hold from string *1/ */
/*   /1*     } *1/ */

/*   /1*     problemstring = problemstringstore; // Restore problem string for rendering normal hold LEDs *1/ */
/*   /1*   } *1/ */

/*   /1*   Serial.println("Problem LEDs:"); *1/ */
/*   /1*   while(true){ // render all normal LEDs (possibly overriding additional LEDs) *1/ */
/*   /1*     int pos = problemstring.indexOf(','); // Hold descriptions are separated by a comma (,) *1/ */
      
/*   /1*     String hold; *1/ */
/*   /1*     if (pos > 0) { // Still holds left in the problem string *1/ */
/*   /1*       hold = problemstring.substring(0, pos); // Extract one hold description *1/ */
/*   /1*     } *1/ */
/*   /1*     else { // Last hold in the problem string *1/ */
/*   /1*       hold = problemstring; *1/ */ 
/*   /1*     } *1/ */

/*   /1*     char holdtype = hold.charAt(0); // Hold descriptions consist of a hold type (S, P, E) ... *1/ */
/*   /1*     int holdnumber = hold.substring(1).toInt(); // ... and a hold number *1/ */
/*   /1*     int lednumber = ledmapping[holdnumber]; *1/ */
/*   /1*     Serial.print(holdtype); *1/ */
/*   /1*     Serial.print(holdnumber); *1/ */
/*   /1*     Serial.print(" --> "); *1/ */
/*   /1*     Serial.print(lednumber); *1/ */
/*   /1*     if (holdtype == 'S') { // Start hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, green); *1/ */
/*   /1*       Serial.println(" (green)"); *1/ */
/*   /1*     } *1/ */
/*   /1*     if (holdtype == 'R' || holdtype =='P') { // Right hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, blue); *1/ */
/*   /1*       Serial.println(" (blue)"); *1/ */
/*   /1*     } *1/ */
/*   /1*     if (holdtype == 'L') { // Left hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, violet); *1/ */
/*   /1*       Serial.println(" (violet)"); *1/ */
/*   /1*     } *1/ */
/*   /1*     if (holdtype == 'M') { // Match hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, pink); *1/ */
/*   /1*       Serial.println(" (pink)"); *1/ */
/*   /1*     } *1/ */
/*   /1*     if (holdtype == 'F') { // Foot hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, cyan); *1/ */
/*   /1*       Serial.println(" (cyan)"); *1/ */
/*   /1*     } *1/ */
/*   /1*     if (holdtype == 'E') { // End hold *1/ */
/*   /1*       strip.SetPixelColor(lednumber, red); *1/ */
/*   /1*       Serial.println(" (red)"); *1/ */
/*   /1*     } *1/ */
      
/*   /1*     if (pos == -1) { // Last hold has been processed! *1/ */
/*   /1*       strip.Show(); // Light up all hold (and additional) LEDs *1/ */
/*   /1*       problemstring = ""; // Reset problem string *1/ */
/*   /1*       useadditionalled = false; // Reset additional LED option *1/ */
/*   /1*       problem_loaded_time = millis(); // Set problem loaded time *1/ */
/*   /1*       state = 0; // Switch to state 0 (wait for new problem string or configuration) *1/ */
/*   /1*       Serial.println("---------\n"); *1/ */
/*   /1*       break; *1/ */
/*   /1*     } *1/ */

/*   /1*     problemstring = problemstring.substring(pos+1, problemstring.length()); // Remove processed hold from string *1/ */
/*   /1*   } *1/ */
/*   /1* } *1/ */

/*   /1* // When "auto turn off" enabled, clear the LEDs after x minutes of inactivity *1/ */
/*   /1* if (AUTO_TURN_OFF && problem_loaded_time && millis() - problem_loaded_time > AUTO_TURN_OFF_MINUTES * 60000) { *1/ */
/*   /1*   Serial.println("---------\n"); *1/ */
/*   /1*   Serial.println("Turning off LEDs due to inactivity"); *1/ */

/*   /1*   strip.ClearTo(black); // Turn off all LEDs in LED string *1/ */
/*   /1*   strip.Show(); // Light up all hold (and additional) LEDs *1/ */
/*   /1*   problem_loaded_time = 0; // Reset problem loaded time *1/ */

/*   /1*   Serial.println("---------\n"); *1/ */
/*   /1* } *1/ */
}
