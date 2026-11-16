_This repo was forked from [ArduinoMoonBoardLED: A Simple Arduino MoonBoard Bluetooth
Low Energy (BLE) LED System](https://github.com/FabianRig/ArduinoMoonBoardLED)._

## Introduction

This repo contains all the info about our MoonBoard build. The code and BOM is heavily
based on the forked project, but lots of changes have been made to support our
controller+toolchain.

After starting to get this working, I also came across
https://github.com/labs-tibox/moonboard-esp32-ledble. I wanted to figure this esp32
stuff out on my own as a little challenge, so I didn't use any of that code. But cool to
see others are forking Uberi's project for their own boards!

This project aims at providing an easy to use solution for building your own MoonBoard
LED system using an esp32-based controller. It is both compatible with a normal
MoonBoard as well as with the MoonBoard Mini.

This project is compatible with the old and the new MoonBoard app. All features (show
beta, show lights above holds) of the app can be used. Multiple users can connect at the
same time (multi user support).

The new MoonBoard app is not the best experience. If the old one doesn't kick you out,
I'd keep using that (I can, friend can't for some reason). Might move to ![boardsesh](
https://github.com/boardsesh/boardsesh) some day.

## Please READ THIS FIRST

- IMPORTANT: If you want a product that just works, please buy the one offered by Moon
  Climbing! This project is neither supported nor endorsed by Moon Climbing!
- This is a project which requires some work and still might not work as well as the
  original.
- To be absolutely clear: This project is provided as-is. I take absolutely no
  responsibility that it works as expected. In fact, it might break at any time.
- Power must be provided to the LEDs when the data wire is connected. It is possible to
  damage the LEDs if only data is connected and the esp32 is powered on. Be mindful of
  this when testing/flashing/etc.

## Thanks

- Shopping list I heavily followed:
  https://github.com/FabianRig/ArduinoMoonBoardLED/issues/32#issuecomment-3632390737
- Library for esp32 BLE:
  https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/UART/UART.ino
- NeoPixelBus: LEDs
- FabianRig for the original project

## Wiring

The LED strip has three wires: +5V, GND and data. Usually, blue is GND ("negative"),
brown is +5V ("positive"), yellow/green is data. Please double-check! You can find a
wiring diagram at the bottom of this page.

Connect the data line to pin 2 of the esp32 (unless you changed it in config.h). It
might be a good idea to use a resistor (e.g. 330 ohms) in the data line! In case a 330
ohms resistor doesn't work, you might try a lower one (e.g. 220 ohms). I used 220 ohm;
330 ohms resulted in flickering LEDs.

Important: Make sure that both the esp32 and the LED string share GND! Simply power both
the LED string and the esp32 by the same power source. Use the USB connection for
flashing and debugging only! If GND is not connected, the LED string will not work.

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

8. Use the MoonBoard app to connect to the esp32 and show the problems on your board!

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

TODO
