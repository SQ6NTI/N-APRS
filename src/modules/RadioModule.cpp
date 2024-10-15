#include "configuration.h"
#include "defaults.h"
#include "modules/RadioModule.h"

static const char *const TAG = "RadioModule";

RadioModule::RadioModule(Scheduler* aScheduler) : tReceiver(aScheduler, this) {
    this->aScheduler = aScheduler;
    //tRadioRx = new Task(TASK_IMMEDIATE, TASK_FOREVER, [this]() { this->receivePacket(); }, &aScheduler, false);
}

bool RadioModule::initialize() {
    int16_t state = RADIOLIB_ERR_NONE;

    spiModule->initializeRadioSPI();

    #if defined(HAS_SX126X)
        #if defined(SX126X_DIO3_TCXO_VOLTAGE)
            float tcxoVoltage = SX126X_DIO3_TCXO_VOLTAGE;
        #else
            float tcxoVoltage = DEFAULT_LORA_TCXO_V;
        #endif
        if (tcxoVoltage == 0)
            ESP_LOGD(TAG, "LoRA TCXO voltage not defined, disabling DIO3 for TCXO");
        else
            ESP_LOGD(TAG, "LoRA TCXO voltage not defined, setting DIO3 reference at %f V\n", tcxoVoltage);

        #if defined(DEFAULT_LORA_USE_LDO)
            bool useRegulatorLDO = true;
        #else
            bool useRegulatorLDO = false;
        #endif
        ESP_LOGD(TAG, "LoRa voltage regulator: %s", useRegulatorLDO ? "LDO" : "DC-DC");

        /* TODO: For now, radio is initialized with some default values.
         * This should be changed to configuration values once configuration loading is completed.
         */
        float loraFrequency = DEFAULT_LORA_FREQ;
        float loraBandwidth = DEFAULT_LORA_BW;
        int8_t loraPower = DEFAULT_LORA_POWER;
        uint8_t loraSF = DEFAULT_LORA_SF;
        uint8_t loraCR = DEFAULT_LORA_CR;
        uint8_t loraSyncWord = DEFAULT_LORA_SYNCWORD;
        uint16_t loraPreambleLen = DEFAULT_LORA_PREAMBLE_LEN;
        bool loraBoostedGain = true;

        ESP_LOGI(TAG, "Initializing LoRa radio interface");
        state = radioInterface.begin(
            loraFrequency,
            loraBandwidth,
            loraSF,
            loraCR,
            loraSyncWord,
            loraPower,
            loraPreambleLen,
            tcxoVoltage,
            useRegulatorLDO
            );
        /* TODO: better error handling, function? */
        if (state != RADIOLIB_ERR_NONE) {
            ESP_LOGI(TAG, "Initialization failed with code: %d", state);
            return false;
        }
        ESP_LOGI(TAG, "LoRa frequency       : %f MHz", loraFrequency);
        ESP_LOGI(TAG, "LoRa bandwidth       : %f kHz", loraBandwidth);
        ESP_LOGI(TAG, "LoRa power           : %d", loraPower);
        ESP_LOGI(TAG, "LoRa spreading factor: %d", loraSF);
        ESP_LOGI(TAG, "LoRa coding rate     : 4:%d", loraCR);

        /* Boosted gain means more sensitivity on the price of bigger power consumption */
        state = radioInterface.setRxBoostedGainMode(loraBoostedGain);
        if (state != RADIOLIB_ERR_NONE) {
            ESP_LOGE(TAG, "Error: LoRa failed to setup boosted gain, code: %d", state);
            return false;
        }

        /* Current limit depends on the actual chip:
         * SX1261 OCP = 60 mA (DS_SX1261-2_V2_1.pdf page 32/114, Table 5-2: OCP Configuration)
         * SX1262 OCP = 60 mA (DS_SX1261-2_V2_1.pdf page 32/114, Table 5-2: OCP Configuration)
         * SX1268 OCP = 140 mA (DS_SX1268_V1.1.pdf page 31/109, Table 5-2: OCP Configuration)
        */
        #if defined(HAS_SX1261)
            state = radioInterface.setCurrentLimit(60.0);
            ESP_LOGD(TAG, "Setting LoRa current limit (OCP) set to 60 mA");
        #elif defined(HAS_SX1262) || defined(HAS_SX1268)
            state = radioInterface.setCurrentLimit(140.0);
            ESP_LOGD(TAG, "Setting LoRa current limit (OCP) set to 140 mA");
        #endif
        if (state == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
            ESP_LOGE(TAG, "Error: LoRa current limit (OCP) is not within the acceptable range");
            return false;
        }

        /* This alternative to SX126x::setDio1Action(void (*func)()) would only work with ESP32.
         * https://forum.arduino.cc/t/interrupt-in-class-esp32/1039326/12
         * Instead, we invoke setDio1Action() from outside the class with RadioModule::attachInterrupt()
         * and the below is therefore not used.
        */
        //attachInterrupt(hal->pinToInterrupt(SX126X_IRQ), std::bind(&RadioModule::interruptHandler, this), hal->GpioInterruptRising);

        /* DIO2 can become a RF switch, this is configurable and true by default.
         * SX1261 (DS_SX1261-2_V2_1.pdf page 83/114, Section 13.3.5 SetDIO2AsRfSwitchCtrl)
         * SX1262 (DS_SX1261-2_V2_1.pdf page 83/114, Section 13.3.5 SetDIO2AsRfSwitchCtrl)
         * SX1268 (DS_SX1268_V1.1.pdf page 80/114, Section 13.3.5 SetDIO2AsRfSwitchCtrl)
        */
        #ifdef SX126X_DIO2_AS_RF_SWITCH
            bool dio2AsRfSwitch = true;
        #else
            bool dio2AsRfSwitch = false;
        #endif
        ESP_LOGD(TAG, "%s DIO2 as RF SX126x RF switch", dio2AsRfSwitch ? "Setting" : "Unsetting", state);
        state = radioInterface.setDio2AsRfSwitch(dio2AsRfSwitch);
        if (state != RADIOLIB_ERR_NONE) {
            ESP_LOGE(TAG, "Error: Failed to set DIO2 RF switch status");
            return false;
        }

        /* Some LoRa modules have pins dedicated for RX and TX enable. */
        #ifndef SX126X_RXEN
            #define SX126X_RXEN RADIOLIB_NC
            ESP_LOGD(TAG, "No LoRa RX Enable pin defined, setting as not connected");
        #endif
        #ifndef SX126X_TXEN
            #define SX126X_TXEN RADIOLIB_NC
            ESP_LOGD(TAG, "No LoRa TX Enable pin defined, setting as not connected");
        #endif
        #if defined(SX126X_RXEN) || defined(SX126X_TXEN)
            radioInterface.setRfSwitchPins(SX126X_RXEN, SX126X_TXEN);
            ESP_LOGD(TAG, "Setting controller pin %d as LoRa RX Enable and %d as LoRa TX Enable, result: %d", SX126X_RXEN, SX126X_TXEN, state);
        #endif
        if (state != RADIOLIB_ERR_NONE) {
            ESP_LOGE(TAG, "Error: There was a problem with LoRa configuration procedure, code: %d", state);
            return false;
        }

        /* CRC is on by default with length of 1 byte. This is just to enable it explicity. */
        state = radioInterface.setCRC(RADIOLIB_SX126X_LORA_CRC_ON);
        if (state == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
            ESP_LOGE(TAG, "Error: Failed to set LoRa CRC settings");
            return false;
        }
    #endif

    return state == RADIOLIB_ERR_NONE;
}

/* This is a workaround to a problem of attaching class member as interrupt handler INSIDE the class itself.
 * https://forum.arduino.cc/t/interrupt-in-class-esp32/1039326/25
*/
void RadioModule::attachInterrupt(void (*callback)()) {
    #if defined(HAS_SX126X)
        radioInterface.setDio1Action(callback);
        ESP_LOGD(TAG, "Interrupt handler attached to DIO1");
    #endif
}

PhysicalLayer* RadioModule::getRadioInterface() {
    return &radioInterface;
}

/* Interrupt Service Routine (ISR) is kept in IRAM to avoid calling FLASH during interrupt handling.
 * Use inside non-capturing lambda, because this is not static.
*/
void IRAM_ATTR RadioModule::interruptHandler(void) {
    if (radioInterface.checkIrq(RADIOLIB_IRQ_RX_DONE)) {
        tReceiver.enable();
    }
}

/* Constructor for the RadioModule's nested class Receiver */
RadioModule::Receiver::Receiver(Scheduler* aScheduler, RadioModule* radioModule) : Task(TASK_IMMEDIATE, TASK_ONCE, aScheduler, false)  {
    
}

/* Handler for receiving packets from radioInterface */
bool RadioModule::Receiver::Callback() {

}
