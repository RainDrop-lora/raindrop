#include <SPI.h>
#include <ModbusMaster.h>
#include <RTCZero.h>

#include "pins.h"
#include "radio.h"

ModbusMaster sensor;

RTCZero rtc;

uint16_t batteryMillivolts = 0;

void alarmMatch() {
}

void preTransmission() {
  rs485DriverEnable();
}

void postTransmission() {
  rs485ReaderEnable();
}

// payload to send to TTN gateway
static uint8_t payload[5];

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 5;

void goToSleep() {
    digitalWrite(LED_WAN, LOW);
    digitalWrite(LED_SENSOR, LOW);
    digitalWrite(LED_BATT, LOW);
    
    digitalWrite(RS_485_EN, LOW);
    digitalWrite(I2C_EN, HIGH);

    digitalWrite(RE, HIGH);
    digitalWrite(DE, LOW);

    rtc.begin();
    rtc.setEpoch(0);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(alarmMatch);

    rtc.setAlarmEpoch(rtc.getEpoch() + TX_INTERVAL);
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    rtc.standbyMode();
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

int16_t temperature;
uint16_t moisture;


void readSensors() {
//  digitalWrite(LED_SENSOR, HIGH);
    rs485Enable();
    delay(30);

    Serial5.begin(19200);
    uint8_t result = sensor.readInputRegisters(0, 2);
    Serial5.end();
//    Serial.print("modbus result: "); Serial.println(result, HEX);
    
    moisture = sensor.getResponseBuffer(0);
    temperature = sensor.getResponseBuffer(1);
    batteryMillivolts = analogRead(A0) * 2 * 3300 / 1024;

//    Serial.print("Battery voltage: ");
//    Serial.println(batteryMillivolts);
//    Serial.print("Temperature: "); Serial.print(temperature);
//    Serial.println(" *C");
//    Serial.print("Moisture ");
//    Serial.println(moisture);

    rs485Disable();
    rs485Sleep();
//  digitalWrite(LED_SENSOR, LOW);
}

void setup() {
    pinMode(DE, OUTPUT);//de
    pinMode(RE, OUTPUT);//~re
    pinMode(LED_WAN, OUTPUT);
    pinMode(LED_SENSOR, OUTPUT);
    pinMode(LED_BATT, OUTPUT);
    pinMode(RS_485_EN, OUTPUT);
    pinMode(I2C_EN, OUTPUT);
    
    rs485Disable();    
    rs485Sleep();
    
    digitalWrite(LED_WAN, HIGH);
    digitalWrite(LED_SENSOR, HIGH);
    digitalWrite(LED_BATT, HIGH);
    delay(500);
    digitalWrite(LED_WAN, LOW);
    digitalWrite(LED_SENSOR, LOW);
    digitalWrite(LED_BATT, LOW);
        
    Serial.begin(9600);

    rtc.begin();
    rtc.setEpoch(0);
    rtc.enableAlarm(rtc.MATCH_YYMMDDHHMMSS);
    rtc.attachInterrupt(alarmMatch);
    
    sensor.begin(1, Serial5);
    sensor.preTransmission(preTransmission);
    sensor.postTransmission(postTransmission);

    Serial.println(F("Starting"));

    lora_init();
    readSensors();
    setupPayload();
    lora_send(payload, 6);
}

void on_tx_start() {
//  digitalWrite(LED_WAN, HIGH);
}

void on_tx_complete() {
//  digitalWrite(LED_WAN, LOW);
  goToSleep();        
  lora_adjust_time();  
  readSensors();
  setupPayload();  
  lora_send(payload, 6);
}

void setupPayload() {
        payload[0] = batteryMillivolts & 0x00FF;
        payload[1] = (batteryMillivolts >> 8) & 0x00FF;
        payload[2] = moisture & 0x00FF;
        payload[3] = (moisture >> 8) & 0x00FF;
        payload[4] = temperature & 0x00FF;
        payload[5] = (temperature >> 8) & 0x00FF;
}

void loop() {
  os_runloop_once();
}
