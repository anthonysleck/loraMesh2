/*
   Lora Mesh - Bridge
   Description:
   See Github Repository
   Notes:
   Contact Info:
   email - anthony.sleck@gmail.com
   web - anthony.sleck.us
   github - https://github.com/anthonysleck/loraMesh2
   Changelog:
   0.1 - new code; seperated code for node/bridge devices
   0.2 - updated code; teseted node/bridge on LORA32 and TBEAM
   0.3 - add'd #define RH_MESH_MAX_MESSAGE_LEN 50 to both node and bridge to increase message length; remv's old commented out unused code.
   0.4 - corrected node links
   0.5 - add'd gps support; remv'd drd; reordered wdreset; remv'd wdreset messages; updated misc code
*/

#ifndef config_h
#define config_h

// system versioning
#define VER "0.5"
#define VER_BUILD "01282024"
#define email "anthony.sleck@gmail.com"
#define firmwareLink "https://github.com/anthonysleck/loraMesh2"

// common includes
#include <Arduino.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include "SSD1306Wire.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <TinyGPSPlus.h>

// temporary testing config; disable all parameters here when in production
String gpsTempLocation = "";
//float gpsLat;
//float gpsLng;

// device definitions
#define DEBUG 1      // set to 0 to disable serial debugging; set to 1 to enable serial debugging
#define HASDISPLAY 1 // set to 0 for no display; set to 1 for display
#define DEV_ID 11    // address of the bridge; can be a value from 11-254
#define BRIDGE_ID 1  // set to default bridge/server to receive data; default is 1; set values from 1-10
#define HASGPS 0     // set to 0 for no gps; set to 12 for gps

// debugging
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

// gps config
#if HASGPS == 1
#define CS 15
#define GPS_RX 34
#define GPS_TX 12
#define GPS_BAUD 9600
SoftwareSerial ss (GPS_RX, GPS_TX);
TinyGPSPlus gps;
#endif

// lora config
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18
#define LORA_RST 23
#define LORA_DI0 26
#define txPower 20
#define txFreq 915.0
#define TXINTERVAL 10000 // delay between successive transmissions
#define RH_MESH_MAX_MESSAGE_LEN 50
unsigned long nextTxTime;
RH_RF95 rf95(LORA_CS, LORA_DI0); // slave select pin and interrupt pin, [heltec|ttgo] ESP32 Lora OLED with sx1276/8
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;
RHMesh manager(rf95, DEV_ID);

// OLED config
#if HASDISPLAY == 1
#define OLED_SDA 21
#define OLED_SCL 22
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);
#endif

// wd timer
unsigned long startPostMillis;
const unsigned long periodPost = 300000; // the time in ms
#define WATCHDOG_TIMEOUT_S 15            // enter time in sec
hw_timer_t *watchDogTimer = NULL;

#endif
