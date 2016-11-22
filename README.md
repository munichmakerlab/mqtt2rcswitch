# MQTT controller for remote controlled power sockets

We have a bunch of remote controlled power sockets at the space, and wanted to control them via our space automation tools.

## Building
### Hardware
* ESP8266
* A 433Mhz transmitter. Look at https://github.com/sui77/rc-switch/wiki/List_TransmitterReceiverModules for examples

We used a WeMos D1 mini board for the ESP8266. It's powered over USB, and provides 5V to the RF module. The Data pin of the module is connected to GPIO 4 (Pin D2), and works just fine with the 3.3V of that pin.

### Software
* Arudino IDE for ESP8266
* [rc-switch](https://github.com/sui77/rc-switch)
* [pubsubclient](https://github.com/knolleary/pubsubclient)

## Usage
Send the following to the MQTT topic defined in ```config.h```, e.g.```mumalab/room/rcsockets```:
```
type,house_code,device_code,action
```

with the options
* ```type``` - Type of RF sockets used, according to https://github.com/sui77/rc-switch/wiki/HowTo_OperateLowCostOutlets (which is missing type D). Only implemented types are "a" and "d" right now.
* ```house_code<``` and ```device_code``` - Whatever is set on the socket as address
* ```action``` - ```1``` for switching on, ```0``` for switching off
