# LoRa - RS485 bridge

This is a board that is specifically designed to easily hook up RS485 and I2C sensors to LoRaWAN network.
![](lora-rs485-bridge.png)

## Features

* Arduino compatible (Adafruit Feather M0 based)
* Low power:
  * Sleep current - 15uA
  * Message cost @ SF7 - 38mC (~190k messages from 4xAA battery)
  * Power switching for external I2C and RS485 sensors
* ESD protection
* Reverse polarity protection
* Wide input voltage range (up to 12V)
