# Lora Mesh

Goal

The primary goal of this project is to create a Lora Mesh using ESP32 boards in conjunction with a GPS to track locations and other various data of racers and eport back to a main host, ie a server. The devices will all be able to custom addressable with a name or number of each individual racer. The server will be able to see data such as location, speed, heading direction, and possibly something along the lines of acceleration or orientation using a 12DOF accelerometer.

What is LoRa®?

LoRa (short for long range) is a spread spectrum modulation technique derived from chirp spread spectrum (CSS) technology. Semtech’s LoRa is a long range, low power wireless platform that has become the de facto wireless platform of Internet of Things (IoT). LoRa devices and networks such as the LoRaWAN® enable smart IoT applications that solve some of the biggest challenges facing our planet: energy management, natural resource reduction, pollution control, infrastructure efficiency, and disaster prevention. Semtech’s LoRa devices have amassed several hundred known uses cases for smart cities, homes and buildings, communities, metering, supply chain and logistics, agriculture, and more. With hundreds of millions of devices connected to networks in more than 100 countries and growing, LoRa is creating a smarter planet.

Future things include the following:
- Full GPS Support
- Ability to write to an SQL database
- Config through WWW

## Supported Hardware

Currently the hardware is limited to the TTGO Lora32 T3 V1.6.1 and the TTGO T-Beam V1.1

![20240126_160008](https://github.com/anthonysleck/loraMesh2/assets/20516767/307da9a6-d086-425d-ba49-58a5e98f3bd5)

##  Usage Instructions

Usage is pretty simple!
1. Downlaod.
2. Open the node type you are flashing in VSCode.
3. Adjust the settings for the node type in the config.h for the device you are using.
4. Compile and upload.

Additional notes
- There has to be atleast one bridge to accept the data. Currently that ID is set to 1. If the bridge id is changed, make sure the BRIDGE_ID is also changed in the node config.h file.
- You can turn serial printing on or off by changing the #define DEBUG to 0 or 1 in the config.h file for the node type. 0 is on, and 1 is off.
- You can turn the display on or off by changing the #define HASDISPLAY to 0 or 1 in the config.h file for the node type. 0 is off and 1 is on.
- Be sure to set different DEV_ID's for each node in the config.h file for the node type.
- For bridges, there is a a Double Reset Detector and the Network Manager enabled by default. These will be able to be disabled at a later date, but are required. You must connect to a WiFi network upon first boot of the device.
- The Bridges also require the filesystem to be uploaded.
- Please note that the BRIDGE_ID and DEV_ID for each node has to be unique and cannot overlap. If bridge is set to 1 the nodes need to be set higher than 1. It is recommended to keep bridges from 1-10 and nodes from 11-254.

##  Changelog

- 0.0 - initial release
- 0.1 - new code; seperated code for node/bridge devices
- 0.2 - updated code; teseted node/bridge on LORA32 and TBEAM
- 0.3 - add'd #define RH_MESH_MAX_MESSAGE_LEN 50 to both node and bridge to increase message length; remv's old commented out unused code.
- 0.4 - corrected node links
- 0.5 - add'd gps support; remv'd drd; reordered wdreset; remv'd wdreset messages; updated misc code

## Authors

- [@anthonysleck](https://www.github.com/anthonysleck)


## License

[GNU General Public License v3.0 ](https://choosealicense.com/licenses/gpl-3.0/)
