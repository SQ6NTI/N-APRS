#include "configuration.h"
#include "defaults.h"
#include "util.h"
#include "RadioModule.h"

static const char *const TAG = "RadioModule";

RadioModule::RadioModule(Scheduler aScheduler) {
    this->aScheduler = aScheduler;
    //tRadioRx = new Task(TASK_IMMEDIATE, TASK_FOREVER, []() { radioModule->receivePacket(); }, &aScheduler, false);
}

bool RadioModule::initialize() {
    int state = RADIOLIB_ERR_NONE;

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
        ESP_LOGD(TAG, "LoRA voltage regulator: %s", useRegulatorLDO ? "LDO" : "DC-DC");

        /* TODO: For now, radio is initialized with some default values.
         * This should be changed to configuration values once configuration loading is completed.
         */
        state = radioInterface.begin(
            DEFAULT_LORA_FREQ,
            DEFAULT_LORA_BW,
            DEFAULT_LORA_SF,
            DEFAULT_LORA_CR,
            DEFAULT_LORA_SYNCWORD,
            DEFAULT_LORA_POWER,
            DEFAULT_LORA_PREAMBLE_LEN,
            tcxoVoltage,
            useRegulatorLDO
            );
        /* TODO: better error handling */
        if (state != RADIOLIB_ERR_NONE)
            return false;

        #if defined(HAS_SX126X)
            /* This alternative to SX126x::setDio1Action(void (*func)()) would only work with ESP32.
             * https://forum.arduino.cc/t/interrupt-in-class-esp32/1039326/12
            */
            //attachInterrupt(hal->pinToInterrupt(SX126X_IRQ), std::bind(&RadioModule::interruptHandler, this), hal->GpioInterruptRising);

            #ifdef SX126X_DIO2_AS_RF_SWITCH
                bool dio2AsRfSwitch = true;
            #else
                bool dio2AsRfSwitch = false;
            #endif
            state = radioInterface.setDio2AsRfSwitch(dio2AsRfSwitch);
            ESP_LOGD(TAG, "%s DIO2 as RF SX126x RF switch: %d", dio2AsRfSwitch ? "Setting" : "Unsetting", state);
        #endif
    #endif

    return true;
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
        rxDoneCallback();
        
    }
}
