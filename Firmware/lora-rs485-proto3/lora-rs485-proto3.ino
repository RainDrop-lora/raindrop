/*******************************************************************************
 * The Things Network - Sensor Data Example
 * 
 * Example of sending a valid LoRaWAN packet with DHT22 temperature and
 * humidity data to The Things Networ using a Feather M0 LoRa.
 * 
 * Learn Guide: https://learn.adafruit.com/the-things-network-for-feather
 * 
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 * Copyright (c) 2018 Brent Rubell, Adafruit Industries
 * 
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *******************************************************************************/
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <ModbusMaster.h>
#include <RTCZero.h>

ModbusMaster sensor;


#define DE 0
#define RE 1

#define LED_WAN 18
#define LED_SENSOR 17
#define LED_BATT 16
#define RS_485_EN 2


RTCZero rtc;

void alarmMatch() {
}

void preTransmission() {
  digitalWrite(RE, HIGH);
  digitalWrite(DE, HIGH);
}

void postTransmission() {
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
}

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

// LoRaWAN AppSKey, application session key
// This should also be in big-endian (aka msb).
static const u1_t PROGMEM APPSKEY[16] = { 0x33, 0x45, 0x61, 0x45, 0xCA, 0x50, 0xEE, 0x43, 0xD4, 0xCC, 0x01, 0x8D, 0xA1, 0xA3, 0xFA, 0x91 };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
// The library converts the address to network byte order as needed, so this should be in big-endian (aka msb) too.
static const u4_t DEVADDR = 0x26011ED3; // <-- Change this address for every node!

// LoRaWAN NwkSKey, network session key
// This should be in big-endian (aka msb).
static const u1_t NWKSKEY[16] = { 0xEB, 0xFA, 0x25, 0x22, 0xE0, 0xAA, 0x6C, 0x00, 0xD4, 0x1B, 0xC9, 0x31, 0x4F, 0x4F, 0xE8, 0xF6 };



// payload to send to TTN gateway
static uint8_t payload[5];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 3;



const lmic_pinmap lmic_pins = {
  .nss = 8,                // Internal connected
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,                // Internal connected
  .dio = {3, 9, LMIC_UNUSED_PIN},    // Connect "i01" to "5"
                    // Connect "D2" to "6"
  .rxtx_rx_active = 0,
  .rssi_cal = 0,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
  .spi_freq = 8000000,
};


void goToSleep() {

//    LMIC_shutdown();
//    SPI.end();
//
//    pinMode(22, INPUT_DISCONNECTED);
//    pinMode(23, INPUT_PULLUP);
//    pinMode(24, INPUT_PULLUP);
//    
//    pinMode(4, INPUT_DISCONNECTED);
//    pinMode(8, INPUT_PULLUP);
//    pinMode(3, INPUT_DISCONNECTED);
//    pinMode(9, INPUT_DISCONNECTED);
//
//    pinMode(LED_WAN, INPUT_DISCONNECTED);
//    pinMode(LED_SENSOR, INPUT_DISCONNECTED);
//    pinMode(LED_BATT, INPUT_DISCONNECTED);
//    pinMode(RS_485_EN, INPUT_DISCONNECTED);
//

    digitalWrite(RE, HIGH);
    digitalWrite(DE, LOW);

//    pinMode(RE, INPUT_DISCONNECTED);
//    pinMode(DE, INPUT_DISCONNECTED);
        

    rtc.setAlarmEpoch(rtc.getEpoch() + TX_INTERVAL);
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    rtc.standbyMode();
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

//---------- WAKEUP!!! ----------------

   
//    pinMode(LED_WAN, OUTPUT);
//    pinMode(LED_SENSOR, OUTPUT);
//    pinMode(LED_BATT, OUTPUT);
//    
//    pinMode(RS_485_EN, OUTPUT);
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            // TTN uses SF9 for its RX2 window.
            LMIC.dn2Dr = EU868_DR_SF9;
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("artKey: ");
              for (int i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                Serial.print(artKey[i], HEX);
              }
              Serial.println("");
              Serial.print("nwkKey: ");
              for (int i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              Serial.print("-");
                      Serial.print(nwkKey[i], HEX);
              }
              Serial.println("");
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
      // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
             Serial.println(F("EV_RFU1"));
             break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
            break;
        case EV_TXCOMPLETE:            
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            digitalWrite(LED_WAN, LOW);
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
          
            goToSleep();
            LMIC.bands[BAND_MILLI].avail = os_getTime();
            LMIC.bands[BAND_CENTI].avail = os_getTime();
            LMIC.bands[BAND_DECI].avail = os_getTime();  
                             
//            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            digitalWrite(LED_WAN, HIGH);
            do_send(&sendjob);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        case EV_SCAN_FOUND:
            Serial.println(F("EV_SCAN_FOUND"));
            break;
        
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            digitalWrite(LED_WAN, HIGH);
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            LMIC_reset();
            do_send(&sendjob);
            break;
    }
}


uint16_t batteryMillivolts = 0;
int16_t temperature;
uint16_t moisture;

void readSensors() {
    digitalWrite(LED_SENSOR, HIGH);
    digitalWrite(RS_485_EN, HIGH);
    delay(100);

    sensor.begin(1, Serial5);
    sensor.preTransmission(preTransmission);
    sensor.postTransmission(postTransmission);
    
    uint8_t result = sensor.readInputRegisters(0, 2);
    Serial.print("modbus result: "); Serial.println(result, HEX);
    
    moisture = sensor.getResponseBuffer(0);
    temperature = sensor.getResponseBuffer(1);
    batteryMillivolts = analogRead(A0) * 2 * 3300 / 1024;

    Serial.print("Battery voltage: ");
    Serial.println(batteryMillivolts);
    Serial.print("Temperature: "); Serial.print(temperature);
    Serial.println(" *C");
    Serial.print("Moisture ");
    Serial.println(moisture);

    digitalWrite(RS_485_EN, LOW);
    digitalWrite(LED_SENSOR, LOW);
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        readSensors();
        payload[0] = batteryMillivolts & 0x00FF;
        payload[1] = (batteryMillivolts >> 8) & 0x00FF;
        payload[2] = moisture & 0x00FF;
        payload[3] = (moisture >> 8) & 0x00FF;
        payload[4] = temperature & 0x00FF;
        payload[5] = (temperature >> 8) & 0x00FF;

        //(port, payload, payload size, ack)
        LMIC_setTxData2(1, payload, 6, 0);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}


void setup() {
    pinMode(DE, OUTPUT);//de
    pinMode(RE, OUTPUT);//~re
    pinMode(LED_WAN, OUTPUT);
    pinMode(LED_SENSOR, OUTPUT);
    pinMode(LED_BATT, OUTPUT);
    
    pinMode(RS_485_EN, OUTPUT);
    digitalWrite(RS_485_EN, HIGH);
    
    digitalWrite(RE, HIGH);
    digitalWrite(DE, LOW);

    digitalWrite(LED_WAN, HIGH);
    digitalWrite(LED_SENSOR, HIGH);
    digitalWrite(LED_BATT, HIGH);
    delay(1000);
    digitalWrite(LED_WAN, LOW);
    digitalWrite(LED_SENSOR, LOW);
    digitalWrite(LED_BATT, LOW);
    delay(1000);
        
    Serial5.begin(19200);
    Serial.begin(115200);

    rtc.begin();
    rtc.setEpoch(0);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(alarmMatch);
    
//    sensor.begin(1, Serial5);
//    sensor.preTransmission(preTransmission);
//    sensor.postTransmission(postTransmission);

    Serial.println(F("Starting"));

    // LMIC init.
    os_init();
    digitalWrite(LED_SENSOR, HIGH);

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset(); 

//    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);

    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.

    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

    // Disable link-check mode and ADR, because ADR tends to complicate testing.
    LMIC_setLinkCheckMode(0);
    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;
    
    // Set data rate and transmit power for uplink
    LMIC_setDrTxpow(DR_SF9, 27);

    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}

void loop() {
  // we call the LMIC's runloop processor. This will cause things to happen based on events and time. One
  // of the things that will happen is callbacks for transmission complete or received messages. We also
  // use this loop to queue periodic data transmissions.  You can put other things here in the `loop()` routine,
  // but beware that LoRaWAN timing is pretty tight, so if you do more than a few milliseconds of work, you
  // will want to call `os_runloop_once()` every so often, to keep the radio running.
  os_runloop_once();
}
