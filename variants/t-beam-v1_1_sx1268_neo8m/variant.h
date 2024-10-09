/* Main microcontroller settings */
#define GPS_RX_PIN                  34      /* ESP32 IO34 (PIN10) = NEO6M/NEO8M TXD1              (PIN20) */
#define GPS_TX_PIN                  12      /* ESP32 IO12 (PIN18) = NEO6M/NEO8M RXD1              (PIN21) */
#define BUTTON_PIN                  38      /* ESP32 IO38 (PIN7 ) = SW5 BUTTON                            */
#define BUTTON_PIN_MASK             GPIO_SEL_38
#define HAS_I2C
#define I2C_SDA                     21      /* ESP32 IO22 (PIN39) = I2C SDA */
#define I2C_SCL                     22      /* ESP32 IO22 (PIN39) = I2C SCL */
#define PMU_IRQ                     35      /* ESP32 IO35 (PIN11) = AXP192 IRQ/WAKEUP             (PIN48) */
#define RADIO_SCLK_PIN              5       /* ESP32 IO5  (PIN34) = LORA32 PIN5  = SX126X SCK     (PIN18) */
#define RADIO_MISO_PIN              19      /* ESP32 IO19 (PIN38) = LORA32 PIN7  = SX126X MISO    (PIN16) */
#define RADIO_MOSI_PIN              27      /* ESP32 IO27 (PIN16) = LORA32 PIN6  = SX126X MOSI    (PIN17) */
#define RADIO_CS_PIN                18      /* ESP32 IO18 (PIN35) = LORA32 PIN4  = SX126X NSS     (PIN19) */
#define RADIO_RST_PIN               23      /* ESP32 IO23 (PIN36) = LORA32 PIN3  = SX126X NRESET  (PIN15) */
#define RADIO_DIO1_PIN              33      /* ESP32 IO32 (PIN12) = LORA32 PIN10 = SX126X DIO1    (PIN13) */
#define RADIO_DIO2_PIN              32      /* ESP32 IO33 (PIN13) = LORA32 PIN9  = SX126X DIO2    (PIN12) */
#define RADIO_DIO0_PIN              26      /* ESP32 IO26 (PIN15) = LORA32 PIN11 = SX126X NO PIN          */
#define BOARD_LED                   4       /* ESP32 IO4  (PIN24) = RED LED                               */
#define LED_ON                      LOW
#define LED_OFF                     HIGH

/* Display component */
#define HAS_DISPLAY

/* GPS component */
#define HAS_GPS
#define HAS_UBLOX
#define GPS_SERIAL                  1
#define GPS_BAUD_RATE               9600

/* Power management component */
#define HAS_PMU
#define HAS_AXP192

/* Radio component */
#define HAS_SX1268
#define HAS_SX126X
#define SX126X_CS RADIO_CS_PIN
#define SX126X_BUSY RADIO_DIO2_PIN          /* DIO2 is used as BUSY pin */
#define SX126X_IRQ RADIO_DIO1_PIN           /* DIO1 is used as IRQ pin */
#define SX126X_RESET RADIO_RST_PIN
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_DIO3_TCXO_VOLTAGE 1.8        /* This may have no effect on this board as IO3 looks not connected */
