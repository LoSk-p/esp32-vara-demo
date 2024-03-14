# ESP32 Vara Demo

## Requirements

* Platformio core ([installation instructions](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html)).
* Any serial client for your OS (`screen` for Linux, for example).

## Installation

Clone the repo:
```bash
git clone https://github.com/LoSk-p/esp32-vara-demo.git
```
## Configuration
Create file `Secret.h` in `src` folder with the following content:
```
// Vara Network ED
#define PRIV_KEY ""
#define SS58_ADR ""
#define PUB_KEY ""

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
```
and fill it with the information about your Vara account and WiFi Network. `PRIV_KEY` and `PUB_KEY` are the private and public keys of your Vara account, `SS58_ADR` is its address.

**This demo works only for ED25519 accounts! Also account must have more then 10 tokens.**

To get the private and public keys from your account seed phrase you can use [get-keys.py](get-keys.py) script. Just run it and follow instructions:
```bash
python3 get-keys.py
```

## Upload
Connect ESP32 to the computer using USB cable and build the project:
```bash
cd esp32-vara-demo
platformio run -t upload
```
This command will build binary files for esp and upload them, so in the end you will see following
```
Writing at 0x000b9def... (84 %)
Writing at 0x000bf4c2... (87 %)
Writing at 0x000c56bf... (90 %)
Writing at 0x000cc6df... (93 %)
Writing at 0x000d1dec... (96 %)
Writing at 0x000d71b0... (100 %)
Wrote 836160 bytes (538555 compressed) at 0x00010000 in 12.2 seconds (effective 548.7 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
======================================================================= [SUCCESS] Took 24.08 seconds =======================================================================
```

## Run

After uploading reconnect ESP to the computer and run your Serial Client (screen with the port `/dev/ttyACM0` in this example):
```bash
screen /dev/ttyACM0 115200
```
You will see the logs.

You can find out the port in the logs arter `platformio run -t upload` command in the previous section. Look for something like this:
```
Auto-detected: /dev/ttyACM0
Uploading .pio/build/nodemcu-32s/firmware.bin
esptool.py v4.5.1
Serial port /dev/ttyACM0
Connecting.......
```