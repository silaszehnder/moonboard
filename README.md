_This repo was forked from [ArduinoMoonBoardLED: A Simple Arduino MoonBoard Bluetooth
Low Energy (BLE) LED System](https://github.com/Uberi/Arduino-HardwareBLESerial)._

## Introduction

This repo contains all the info about our MoonBoard build. The code and BOM is heavily
based on the forked proejct, but lots of changes have been made to support our
controller+toolchain.

This project aims at providing an easy to use solution for building your own MoonBoard
LED system using an Arduino or Arduino-compatible controller. It is both compatible with
a normal MoonBoard as well as with the MoonBoard Mini.

This project is compatible with the "new" MoonBoard app. All features (show beta, show
lights above holds) of the app can be used. Multiple users can connect at the same time
(multi user support).

Update (2026): It is compatible with the updated MoonBoard app version! However, many
users report general problems with the app (both on Android as well as on Apple
devices). In case of problems you might try ![boardsesh](
https://github.com/boardsesh/boardsesh) as an alternative.

![mini_benchmark](
https://user-images.githubusercontent.com/88741530/129411463-636b222b-e963-4542-a1dd-b33eae562bf2.jpg)

## Please READ THIS FIRST

- IMPORTANT: If you want a product that just works, please buy the one offered by Moon
  Climbing! This project is neither supported nor endorsed by Moon Climbing!
- This is a project which requires some work and still might not work as well as the
  original.
- To be absolutely clear: This project is provided as-is. I take absolutely no
  responsibility that it works as expected. In fact, it might break at any time. You
  have been warned!
- Never power the Arduino only without powering the LEDs when the data wire is connected
  to the LED string! This may destroy the first LED! Just use the same power source for
  both of them and don't disconnect it.

## Thanks

All the heavy lifting in this project is done by two awesome libraries: NeoPixelBus (for
the LED string) and HardwareBLESerial (for BLE functionality). They make it possible to
keep this project quite short, easy to understand, and easily maintainable.

## Wiring

The LED strip has three wires: +5V, GND and data. Usually, blue is GND ("negative"),
brown is +5V ("positive"), yellow/green is data. Please double-check! You can find a
wiring diagram at the bottom of this page.

Connect the data line to pin D2 of the Arduino (unless you changed it in config.h). It
might be a good idea to use a resistor (e.g. 330 ohms) in the data line! In case a 330
ohms resistor doesn't work, you might try a lower one (e.g. 220 ohms).

Important: Make sure that both the Arduino and the LED string share GND! Simply power
both the LED string and the Arduino by the same power source. Use the USB connection for
flashing and debugging only! If GND is not connected, the LED string will not work.

## How to use (Windows and Linux via a Package Manager)

1. Download and install Visual Studio Code.
2. Install PlatformIO in Visual Studio Code.
3. Download and open this project. To do so, either download it as a zip file or git
   clone it.
4. Adjust config.h settings to your needs (Moonboard type, brightness).
5. Compile and flash to an Arduino Nano 33 BLE.
6. Use the MoonBoard app to connect to the Arduino and show the problems on your board!

## How to use (Linux, command line)

1. Install arduino-cli
2. Install your board's core

```
arduino-cli core install esp32:esp32
```

3. Install libraries

```
arduino-cli lib install "NeoPixelBus by Makuna"
arduino-cli lib install HardwareBLESerial
```

4. Adjust config.h

Follow the instructions in the file ~/src/ArduinoMoonBoardLED/src/config.h in order to
adjust the configuration to your setup.

5. Compile, upload, and monitor

```
arduino-cli compile -b esp32:esp32:esp32 .
arduino-cli upload -p /dev/ttyUSB0 -b esp32:esp32:esp32 .
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200 -b esp32:esp32:esp32
```

8. Use the MoonBoard app to connect to the Arduino and show the problems on your board!

## Advanced: LED Mapping
The most common LED wiring pattern goes like this (front view):
- start bottom left (A1),
- up the column (to A12),
- one column to the right (to B12),
- all the way down (to B1),
- one column to the right (to C1),
- and repeat.

The MoonBoard App encodes holds in the same way. Hold A1 is 0, hold A2 is 1, hold A3 is
2 and so on.

If you need to wire differently, you'll need to adjust the ledmapping[] array in
config.h which maps hold numbers to LED numbers. The standard mapping is n-->n (e.g.
hold #0 is LED #0, hold #1 is LED #1 and so on). You can find an example for a custom
mapping in config.h: The first few LEDs are skipped, then 24 LEDs in a row are used (for
a MoonBoard Mini), then 2 LEDs are skipped (to get across a wooden beam at the back)
etc. Adjust this to your needs if you have a custom wiring and/or want to skip LEDs to
facilitate wiring your LEDs at the back of your board.

## Pictures
![led_test](
https://user-images.githubusercontent.com/88741530/129411527-84e11098-1192-4a29-a052-b712ad3ca17c.jpg)
![mini_back](
https://user-images.githubusercontent.com/88741530/129411542-174241cc-c898-45f9-965f-cb75b34a1869.jpg)
![box](
https://user-images.githubusercontent.com/88741530/129411548-5262a5c9-6b06-4bc8-9d68-e9e363823fdb.jpg)
![arduino_wiring](
https://user-images.githubusercontent.com/88741530/129411556-ead82345-6687-478b-b9a4-95f1d1ac40c2.jpg)
![wiring](
https://github.com/FabianRig/ArduinoMoonBoardLED/assets/88741530/91c049ea-f41a-45a3-8c80-8d3ef9820e9d)
