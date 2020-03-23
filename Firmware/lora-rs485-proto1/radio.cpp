#include <Arduino.h>
#include "radio.h"
#include "pins.h"

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8] = { 0x67, 0x29, 0x02, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = { 0x98, 0xD0, 0x0B, 0x61, 0xAD, 0x38, 0x5A, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { 0xB7, 0xF1, 0x29, 0x67, 0xA8, 0x49, 0x4F, 0xDB, 0x50, 0xA5, 0xAB, 0xC3, 0xCC, 0x40, 0xD1, 0xFA };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}


// LoRaWAN AppSKey, application session key
// This should also be in big-endian (aka msb).
static const u1_t PROGMEM APPSKEY[16] = { 0x4F, 0x8D, 0xDE, 0x82, 0x47, 0x9F, 0x43, 0xAD, 0xD1, 0x73, 0x8A, 0x02, 0x21, 0x88, 0xDD, 0x69 };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
// The library converts the address to network byte order as needed, so this should be in big-endian (aka msb) too.
static const u4_t DEVADDR = 0x260110F0; // <-- Change this address for every node!

// LoRaWAN NwkSKey, network session key
// This should be in big-endian (aka msb).
static const u1_t NWKSKEY[16] = { 0x80, 0x42, 0xD6, 0x39, 0xB3, 0xDC, 0x1B, 0x03, 0x9D, 0x67, 0xA5, 0x98, 0x94, 0xC1, 0x49, 0x6F };

const lmic_pinmap lmic_pins = {
  .nss = RFM_NSS,                // Internal connected
  .rxtx = LMIC_UNUSED_PIN,
  .rst = RFM_RST,                // Internal connected
  .dio = {RFM_IRQ, RFM_DIO01, LMIC_UNUSED_PIN},    
  .rxtx_rx_active = 0,
  .rssi_cal = 0,
  .spi_freq = 8000000,
};

void lora_init() {
      // LMIC init.
    os_init();
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
}
void lora_send(uint8_t* payload, uint8_t size) {
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        //(port, payload, payload size, ack)
        LMIC_setTxData2(1, payload, size, 0);
    }
}

void lora_adjust_time() {
  LMIC.bands[BAND_MILLI].avail = os_getTime();
  LMIC.bands[BAND_CENTI].avail = os_getTime();
  LMIC.bands[BAND_DECI].avail = os_getTime();  
}

void onEvent (ev_t ev) {
//    Serial.print(os_getTime());
//    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
//            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
//            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
//            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
//            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
//            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
//            Serial.println(F("EV_JOINED"));
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
//            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
//            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:            
//            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
//              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }

            on_tx_complete();

            break;
        case EV_LOST_TSYNC:
//            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
//            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
//            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
//            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
//            Serial.println(F("EV_LINK_ALIVE"));
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
//            Serial.println(F("EV_TXSTART"));
            on_tx_start();
            break;
        default:
//            Serial.print(F("Unknown event: "));
//            Serial.println((unsigned) ev);
            lora_init();
            on_tx_complete();
            break;
    }
}
