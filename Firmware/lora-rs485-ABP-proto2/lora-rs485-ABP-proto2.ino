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

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8] = { 0x67, 0x29, 0x02, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = { 0xBC, 0xD5, 0x25, 0xDA, 0x7B, 0xEE, 0x5E, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { 0xB7, 0xF1, 0x29, 0x67, 0xA8, 0x49, 0x4F, 0xDB, 0x50, 0xA5, 0xAB, 0xC3, 0xCC, 0x40, 0xD1, 0xFA };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}



// LoRaWAN AppSKey, application session key
// This should also be in big-endian (aka msb).
static const u1_t PROGMEM APPSKEY[16] = { 0xB5, 0x72, 0x91, 0xEB, 0x83, 0x4F, 0x35, 0xC3, 0x31, 0x8C, 0x4F, 0xE2, 0x92, 0xA9, 0x6A, 0xE1 };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
// The library converts the address to network byte order as needed, so this should be in big-endian (aka msb) too.
static const u4_t DEVADDR = 0x26011C31; // <-- Change this address for every node!

// LoRaWAN NwkSKey, network session key
// This should be in big-endian (aka msb).
static const u1_t NWKSKEY[16] = { 0x20, 0x75, 0xF8, 0x4B, 0xB2, 0x21, 0xE8, 0xDE, 0x65, 0x62, 0xEC, 0x7A, 0x4C, 0x98, 0x7A, 0x42 };


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
  .dio = {3, 9, LMIC_UNUSED_PIN},    
  .rxtx_rx_active = 0,
  .rssi_cal = 0,
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
//    pinMode(RE, INPUT_DISCONNECTED);
//    pinMode(DE, INPUT_DISCONNECTED);
        
    digitalWrite(LED_SENSOR, LOW);

    rtc.setAlarmEpoch(rtc.getEpoch() + TX_INTERVAL);
    rtc.standbyMode();

//---------- WAKEUP!!! ----------------

    digitalWrite(LED_SENSOR, HIGH);
   
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
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
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

void readSensors() {
    digitalWrite(RS_485_EN, HIGH);
    delay(100);
    batteryMillivolts = analogRead(A0) * 2 * 3300 / 1024;
    Serial.println(batteryMillivolts);
    digitalWrite(RS_485_EN, LOW);
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        readSensors();
        payload[0] = batteryMillivolts & 0x00FF;
        payload[1] = (batteryMillivolts >> 8) & 0x00FF;

        //(port, payload, payload size, ack)
        LMIC_setTxData2(1, payload, 2, 0);
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
    
    digitalWrite(RE, LOW);
    digitalWrite(DE, HIGH);
    delay(1000);
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
        
    Serial1.begin(19200);
    Serial.begin(9600);
    rtc.begin();
    rtc.setEpoch(0);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(alarmMatch);
    
//    sensor.begin(1, Serial1);
//    sensor.preTransmission(preTransmission);
//    sensor.postTransmission(postTransmission);

    Serial.println(F("Starting"));

    // LMIC init.
    os_init();
    digitalWrite(LED_SENSOR, HIGH);
    
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

//    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

    LMIC_setSession (0x13, DEVADDR, (xref2u1_t)NWKSKEY, (xref2u1_t)APPSKEY);

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

    // Disable link check validation
    LMIC_setLinkCheckMode(0);
    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink
    LMIC_setDrTxpow(DR_SF11, 27);

    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}
