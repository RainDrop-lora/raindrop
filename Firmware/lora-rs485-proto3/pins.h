#define DE          0
#define RE          1
#define LED_WAN     18
#define LED_SENSOR  17
#define LED_BATT    16
#define RS_485_EN   2
#define I2C_EN      5

#define RFM_NSS     8
#define RFM_RST     4
#define RFM_IRQ     3
#define RFM_DIO01   9

#define i2cEnable() digitalWrite(I2C_EN, LOW)
#define i2cDisable() digitalWrite(I2C_EN, HIGH)
#define rs485Enable() digitalWrite(RS_485_EN, HIGH)
#define rs485Disable() digitalWrite(RS_485_EN, LOW)
#define rs485Sleep() digitalWrite(RE, HIGH); digitalWrite(DE, LOW)
#define rs485DriverEnable()   digitalWrite(RE, HIGH); digitalWrite(DE, HIGH)
#define rs485ReaderEnable()   digitalWrite(DE, LOW); digitalWrite(RE, LOW)
