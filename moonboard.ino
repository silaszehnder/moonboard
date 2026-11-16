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

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#ifdef GRB
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
#else
NeoPixelBus<NeoRgbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
#endif

RgbColor red(BRIGHTNESS, 0, 0);
RgbColor green(0, BRIGHTNESS, 0);
RgbColor blue(0, 0, BRIGHTNESS);
RgbColor yellow(ADDITIONAL_LED_BRIGHTNESS, ADDITIONAL_LED_BRIGHTNESS, 0);
RgbColor cyan(0, BRIGHTNESS, BRIGHTNESS);
RgbColor pink(BRIGHTNESS, 0, BRIGHTNESS/2);
RgbColor violet(BRIGHTNESS/2, 0, BRIGHTNESS);
RgbColor black(0);

int problem_loaded_time = 0; // Variable to store the time when a problem was loaded

void handleLed(String hold, bool isAdditional) {
    char holdtype = hold.charAt(0); // Hold descriptions consist of a hold type (S, P, E) ...
    int holdnumber = hold.substring(1).toInt(); // ... and a hold number
    int lednumber = ledmapping[holdnumber];
    String ledType = isAdditional ? "additional" : "regular";

    Serial.print("Processing " + ledType + " for " + hold);

    if (isAdditional) {
        int additionallednumber = lednumber + additionalledmapping[holdnumber];
        if (additionalledmapping[holdnumber] != 0) {
            Serial.print(" --> ");
            Serial.print(additionallednumber);
            if (holdtype == 'S') { // Start hold
                strip.SetPixelColor(additionallednumber, yellow);
                Serial.println(" (yellow)");
            }
            // Right, left, match, or foot hold
            if (holdtype == 'R' || holdtype == 'L' || holdtype == 'M' || holdtype =='F' || holdtype =='P') {
                strip.SetPixelColor(additionallednumber, yellow);
                Serial.println(" (yellow)");
            }
        }        
        else {
            // Finish holds don't get an additional LED!
            Serial.println(" --> Finish hold, no additional LED");
        }
        return;
    }

    // regular
    Serial.print(" --> ");
    Serial.print(lednumber);

    if (holdtype == 'S') { // Start hold
        strip.SetPixelColor(lednumber, green);
        Serial.println(" (green)");
    }
    if (holdtype == 'R' || holdtype =='P') { // Right hold
        strip.SetPixelColor(lednumber, blue);
        Serial.println(" (blue)");
    }
    if (holdtype == 'L') { // Left hold
        strip.SetPixelColor(lednumber, violet);
        Serial.println(" (violet)");
    }
    if (holdtype == 'M') { // Match hold
        strip.SetPixelColor(lednumber, pink);
        Serial.println(" (pink)");
    }
    if (holdtype == 'F') { // Foot hold
        strip.SetPixelColor(lednumber, cyan);
        Serial.println(" (cyan)");
    }
    if (holdtype == 'E') { // End hold
        strip.SetPixelColor(lednumber, red);
        Serial.println(" (red)");
    }
}

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
            Serial.println();
            Serial.println("*********");
            Serial.print("Received Value: ");
            Serial.println(rxValue);
        }

        int firstHashIdx = rxValue.indexOf("#");
        String configVals = rxValue.substring(0, firstHashIdx);
        String holdVals = rxValue.substring(firstHashIdx + 1, rxValue.lastIndexOf("#"));

        Serial.print("Config: ");
        Serial.println(configVals);
        Serial.print("Holds: ");
        Serial.println(holdVals);
        Serial.println();
        Serial.println("*********");

        // clear current problem
        strip.ClearTo(black);
        strip.Show();

        // deal w config
        bool additionalLed = false;
        if (configVals.indexOf('D') != -1) {
            additionalLed = true;
        }

        // deal w holds
        char delimiter = ',';
        int startIndex = 0;
        int endIndex = holdVals.indexOf(delimiter);

        // additional leds. kinda ugly to have two loops, but regular
        // leds need to overrule additional leds and I don't want to 
        // maintain state

        do {
            endIndex = holdVals.indexOf(delimiter, startIndex);
            String part = holdVals.substring(startIndex, endIndex);

            handleLed(part, true);

            startIndex = endIndex + 1;
        } while (endIndex != -1);

        // regular leds
        do {
            endIndex = holdVals.indexOf(delimiter, startIndex);
            String part = holdVals.substring(startIndex, endIndex);

            handleLed(part, false);

            startIndex = endIndex + 1;
        } while (endIndex != -1);

        strip.Show(); // Light up all hold (and additional) LEDs
        problem_loaded_time = millis(); // Set problem loaded time
        Serial.println("---------\n");

    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing BLEDevice");

    // Create the BLE Device
    BLEDevice::init(BLUETOOTH_NAME);

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

    strip.Begin(); // Initialize LED strip
    strip.Show(); // Good practice to call Show() in order to clear all LEDs

    delay(500);
    // Test LEDs by cycling through the colors and then turning the LEDs off again
    strip.SetPixelColor(0, green);
    for (int i = 0; i < PixelCount; i++) {
        strip.ShiftRight(1);
        strip.Show();
        delay(10);
    }
    strip.SetPixelColor(0, blue);
    for (int i = 0; i < PixelCount; i++) {
        strip.ShiftRight(1);
        strip.Show();
        delay(10);
    }
    strip.SetPixelColor(0, yellow);
    for (int i = 0; i < PixelCount; i++) {
        strip.ShiftRight(1);
        strip.Show();
        delay(10);
    }

    delay(2000);
    strip.ClearTo(black);
    strip.Show();
}

void loop() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);                   // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();  // restart advertising
        Serial.println("Started advertising again...");
        oldDeviceConnected = false;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = true;
    }

    // TODO: Add auto turn off
    /* // When "auto turn off" enabled, clear the LEDs after x minutes of inactivity */
    /* if (AUTO_TURN_OFF && problem_loaded_time && millis() - problem_loaded_time > AUTO_TURN_OFF_MINUTES * 60000) { */
    /*     Serial.println("---------\n"); */
    /*     Serial.println("Turning off LEDs due to inactivity"); */

    /*     strip.ClearTo(black); // Turn off all LEDs in LED string */
    /*     strip.Show(); // Light up all hold (and additional) LEDs */
    /*     problem_loaded_time = 0; // Reset problem loaded time */

    /*     Serial.println("---------\n"); */
    /* } */
}
