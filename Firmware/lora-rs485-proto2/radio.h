#ifndef __RADIO_H
#define __RADIO_H

#include <lmic.h>
#include <hal/hal.h>

void on_tx_start();
void on_tx_complete();
void lora_init();
void lora_send(uint8_t* payload, uint8_t size);
void lora_adjust_time();
#endif
