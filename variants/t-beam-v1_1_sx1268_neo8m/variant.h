#define GPS_RX_PIN                  34
#define GPS_TX_PIN                  12
#define BUTTON_PIN                  38
#define BUTTON_PIN_MASK             GPIO_SEL_38
#define I2C_SDA                     21
#define I2C_SCL                     22
#define PMU_IRQ                     35

#define RADIO_SCLK_PIN               5
#define RADIO_MISO_PIN              19
#define RADIO_MOSI_PIN              27
#define RADIO_CS_PIN                18
#define RADIO_DI0_PIN               26
#define RADIO_RST_PIN               23
#define RADIO_DIO1_PIN              33
#define RADIO_BUSY_PIN              32

#define BOARD_LED                   4
#define LED_ON                      LOW
#define LED_OFF                     HIGH

#define GPS_SERIAL                  1
#define GPS_BAUD_RATE               9600
#define HAS_GPS
#define HAS_I2C
#define HAS_DISPLAY
//#define HAS_PMU
#define HAS_AXP192
#define HAS_SX1268

#ifdef HAS_SX1268
#define HAS_SX126x
#endif

#ifdef HAS_SX126x
#define SX126X_CS RADIO_CS_PIN
#define SX126X_DIO1 RADIO_DIO1_PIN
#define SX126X_BUSY RADIO_BUSY_PIN
#define SX126X_RESET RADIO_RST_PIN
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_DIO3_TCXO_VOLTAGE 1.8
#endif



