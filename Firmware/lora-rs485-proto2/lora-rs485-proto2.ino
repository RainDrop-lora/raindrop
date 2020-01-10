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
static const u1_t PROGMEM APPEUI[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x29, 0x67 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = { 0x92, 0x6A, 0xA0, 0xC2, 0xAF, 0x2D, 0x7D, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { 0x22, 0xB0, 0x7C, 0x46, 0x58, 0xD3, 0x55, 0xD9, 0x4C, 0x57, 0x5C, 0xB0, 0x29, 0x26, 0x87, 0x4E };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

// payload to send to TTN gateway
static uint8_t payload[5];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 5;



const lmic_pinmap lmic_pins = {
  .nss = 8,                // Internal connected
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,                // Internal connected
  .dio = {3, 9, LMIC_UNUSED_PIN},    // Connect "i01" to "5"
                    // Connect "D2" to "6"
  .rxtx_rx_active = 0,
  .rssi_cal = 8,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
  .spi_freq = 8000000,
};

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
          
//          rtc.setAlarmEpoch(rtc.getEpoch() + TX_INTERVAL);
//          rtc.standbyMode();
          
            
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            //delay(5000);
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

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        uint8_t result = sensor.readInputRegisters(0, 2);
        Serial.print("req result:");
        Serial.println(result, HEX);

        float moisture = sensor.getResponseBuffer(0);
        float temperature = (float)(sensor.getResponseBuffer(1))/10;

        delay(100);
        result = sensor.writeSingleRegister(4, 30);
        Serial.print("sleep command result:");
        Serial.println(result, HEX);
        digitalWrite(RE, HIGH);
        digitalWrite(DE, LOW);

        Serial.print("Temperature: "); Serial.print(temperature);
        Serial.println(" *C");
        
        // adjust for the f2sflt16 range (-1 to 1)
        temperature = temperature / 100; 

        // read the humidity from the DHT22
        Serial.print("Moisture ");
        Serial.println(moisture);
        // adjust for the f2sflt16 range (-1 to 1)
        moisture = moisture / 1024;
        
        // float -> int
        // note: this uses the sflt16 datum (https://github.com/mcci-catena/arduino-lmic#sflt16)
        uint16_t payloadTemp = LMIC_f2sflt16(temperature);
        // int -> bytes
        byte tempLow = lowByte(payloadTemp);
        byte tempHigh = highByte(payloadTemp);
        // place the bytes into the payload
        payload[0] = tempLow;
        payload[1] = tempHigh;

        // float -> int
        uint16_t payloadMoist = LMIC_f2sflt16(moisture);
        // int -> bytes
        byte moistLow = lowByte(payloadMoist);
        byte moistHigh = highByte(payloadMoist);
        payload[2] = moistLow;
        payload[3] = moistHigh;

        // prepare upstream data transmission at the next possible time.
        // transmit on port 1 (the first parameter); you can use any value from 1 to 223 (others are reserved).
        // don't request an ack (the last parameter, if not zero, requests an ack from the network).
        // Remember, acks consume a lot of network resources; don't ask for an ack unless you really need it.
        LMIC_setTxData2(1, payload, 0, 0);
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
        
    Serial1.begin(19200);
    Serial.begin(115200);
    rtc.begin();
    rtc.setEpoch(0);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(alarmMatch);
    
    sensor.begin(1, Serial1);
    sensor.preTransmission(preTransmission);
    sensor.postTransmission(postTransmission);

    Serial.println(F("Starting"));

    // LMIC init.
    os_init();
    digitalWrite(LED_WAN, HIGH);
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset(); 
//    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
//    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
//    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

    // Disable link-check mode and ADR, because ADR tends to complicate testing.
    LMIC_setLinkCheckMode(0);
    
    // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, and it
    // minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
    LMIC_setDrTxpow(EU868_DR_SF9, 14);

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
