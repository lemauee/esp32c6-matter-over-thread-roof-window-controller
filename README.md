# ESP32 C6 Matter Over Thread Roof Window Controller

Smart home matter over thread roof window controller based on an Espressif ESP32 C6 EVB from Olimex. I use this via a Home Assistant ZBT-1 / Skyconnect running as Thread boarder router. Uses an AM2301 (Similar to DHT11/DHT21) temperature/humidity sensor.

## Setup

### ESP-IDF

https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/linux-macos-setup.html

### ESP Matter

https://docs.espressif.com/projects/esp-matter/en/latest/esp32c6/developing.html

### VSCode extension

https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md

https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/C_CPP_CONFIGURATION.md

Build the [light example](https://github.com/espressif/esp-matter/blob/6a2654a/examples/light/README.md) as a first test. To build the thread version of it, use the `sdkconfig.defaults.c6_thread` by renaming it to `sdkconfig.defaults.esp32c6`. See the [API-Guides](https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32c6/api-guides/build-system.html#target-dependent-sdkconfig-defaults) for further info. Flash it and commission the device to your thread network. Setup of a thread network using the Home Assistant ZBT-1 / Skyconnect can be found in the next section.

### Optional: Home assistant with ZBT-1 / Skyconnect open thread border router

[Thread Setup](https://connectzbt1.home-assistant.io/procedures/enable-thread/) 
[Matter Setup](https://www.home-assistant.io/integrations/matter/)

#### Sync thread credentials

Taken from [a reddit post](https://community.home-assistant.io/t/open-thread-border-router-or-apple-google/691903/2).

> On the Android Companion App open Settings → Companion App → Troubleshooting (I think that is what it is called in english, but it list logfiles and other tools to diagnostics as description of the item).
> In this next window click the Sync Thread Login Credentials (Again it might not be named exactly that, but close to and there should not be that many options and it should be in the bottom)
> It should say either something with “Added …” or “Already using the same credentials”.
> The trick here is to run it again and make sure that it does not say “Added …” the second time.

#### Commission your device

Scan the QR Code from the ESP Matter documentation to initiate. For Home Assistant, do this using the companion app (Add device -> Matter).

#### View your thread network

Enable the open thread web port in the open thread border router addon. Navigate to this webpage and click the topology sidebar entry.

## Build & Flash

0. Source ESP IDF and ESP Matter: `cd esp-idf` `source ./export.sh` `cd esp-matter` `source ./export.sh`
1. Switch to this repos root: `cd esp32c6-matter-over-thread-roof-window-controller`
2. Set target: `idf.py set-target esp32c6`
3. Build: `idf.py build`
4. Plug in your board and erase flash: `idf.py erase_flash`
5. Flash & monitor console: `idf.py flash monitor`

