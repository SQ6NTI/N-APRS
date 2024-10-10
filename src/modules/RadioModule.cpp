#include "configuration.h"
#include "defaults.h"
#include "RadioModule.h"

static const char *const TAG = "RadioModule";

RadioModule *radioModule;

RadioModule::RadioModule() {

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

        #ifdef SX126X_DIO2_AS_RF_SWITCH
            bool dio2AsRfSwitch = true;
        #else
            bool dio2AsRfSwitch = false;
        #endif
        state = radioInterface.setDio2AsRfSwitch(dio2AsRfSwitch);
        ESP_LOGD(TAG, "%s DIO2 as RF LoRa RF switch: %d", dio2AsRfSwitch ? "Setting" : "Not setting", state);
    #endif
}

PhysicalLayer* RadioModule::getRadioInterface() {
    return &radioInterface;
}
