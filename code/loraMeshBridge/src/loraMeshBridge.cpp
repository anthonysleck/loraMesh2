// includes
#include "config.h"

void IRAM_ATTR watchDogInterrupt()
{
    debugln("reboot");
    ESP.restart();
}

void watchDogRefresh()
{
    timerWrite(watchDogTimer, 0); // reset timer
}

void startWatchdogTimer()
{
    debugln("Initializing Watchdog Timer!");
    watchDogTimer = timerBegin(2, 80, true);
    timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
    timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
    timerAlarmEnable(watchDogTimer);
    debugln("Watchdog timer Initialized!");
}

void startDisplay()
{
// start tft if tft available
#if HASDISPLAY == 1
    // start tft
    display.init();
    display.flipScreenVertically();

    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(23, 25, "Starting up!");
    display.display();

    // add short delay
    delay(5000);

    // update tft
    display.init();
    display.flipScreenVertically();

    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(13, 25, "System is on!");
    display.display();

    // add a small delay before switching to DEV_ID
    delay(5000);

    // display device id
    String devID = String(DEV_ID);
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(5, 25, "Bridge ID:");
    display.drawString(90, 25, devID);
    display.display();
#endif
}

/*
void startLittleFS()
{
    // initialize littlefs
    if (!LittleFS.begin())
    {
        debugln("Failed to mount LittleFS");
        while (1)
            ;
    }
}

void startDRD()
{
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);

    if (drd->detectDoubleReset())
    {
        debugln("Double Reset Detected");
        digitalWrite(LED_PIN, LED_ON);
    }
    else
    {
        debugln("No Double Reset Detected");
        digitalWrite(LED_PIN, LED_OFF);
    }
}

void startWIFIManager()
{
// clear display
#if HASDISPLAY == 1
    {
        String devID = String(DEV_ID);
        display.clear();
        display.setFont(ArialMT_Plain_16);
        display.drawString(10, 25, "Bridge ID:");
        display.drawString(100, 25, devID);
        display.display();
    }
#endif

    // set WiFiManager timeout
    wifiManager.setTimeout(180);

    // initialize WiFiManager
    if (!wifiManager.autoConnect("ESP32-Config"))
    {
        debugln("Failed to connect, we should reset and try again...");
        delay(3000);
        ESP.restart();
        delay(5000);
    }
}
*/

void startLora()
{
    debug(F("Initializing Bridge"));
    debugln(DEV_ID);
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    if (!manager.init())
    {
        debugln("Initialization failed");
    }
    else
    {
        debugln("Initialization Complete!");
    }

    rf95.setTxPower(txPower, false); // with false output is on PA_BOOST, power from 2 to 20 dBm, use this setting for high power demos/real usage
    rf95.setFrequency(txFreq);
    rf95.setCADTimeout(500);

    // long range configuration requires for on-air time
    boolean longRange = false;
    if (longRange)
    {
        // custom configuration
        RH_RF95::ModemConfig modem_config = {
            0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
            0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
            0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
        };
        rf95.setModemRegisters(&modem_config);
    }
    else
    {
        // Predefined configurations( bandwidth, coding rate, spread factor ):
        // Bw125Cr45Sf128     Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
        // Bw500Cr45Sf128     Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
        // Bw31_25Cr48Sf512   Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
        // Bw125Cr48Sf4096    Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
        // Bw125Cr45Sf2048    Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
        if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128))
        {
            debugln(F("set config failed"));
        }
    }
    debugln("RF95 ready");
}

uint8_t data[] = "Gotcha!"; // change response message
uint8_t len = sizeof(buf);
uint8_t from;

void messageRefresh()
{
    if (manager.recvfromAck(buf, &len, &from))
    {
        debug("Data from node n.");
        debug(from);
        debug(": ");
        debug((char *)buf);
        debug(" rssi: ");
        debugln(rf95.lastRssi());

        // Send a reply back to the originator client
        res = manager.sendtoWait(data, sizeof(data), from);
        if (res != RH_ROUTER_ERROR_NONE)
        {
            debug("sendtoWait failed:");
            debugln(res);
        }
    }
}

void setup()
{
    // start serial
    Serial.begin(115200);
    delay(1000);

    // start tft
    startDisplay();

    // print sketch information
    debugln("Created by Anthony Sleck");
    debugln("Email at anthony.sleck@gmail.com");
    debug("Version ");
    debugln(VER);
    debug("Build Code ");
    debugln(VER_BUILD);
    debug("Github: ");
    debugln(firmwareLink);

    // initialize littlefs
    //startLittleFS();

    // start double reset detector
    //startDRD();

    // start WiFIManager
    //startWIFIManager();

    // display WiFi status
    //debugln("Connected to WiFi");

    // start lora
    startLora();

    // start watchdog timer
    startWatchdogTimer();
}

void loop()
{
    // reset watchdog timer
    watchDogRefresh();

    // refresh messages
    messageRefresh();

    // double reset detector logic
    //drd->loop();
}