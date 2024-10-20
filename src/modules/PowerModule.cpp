#include "configuration.h"
#include "defaults.h"
#include "PowerModule.h"

static const char *const TAG = "PowerModule";

#if defined(HAS_AXP192)
    AXP20X_Class axp;
#endif

PowerModule::PowerModule() {
    
}

bool PowerModule::initialize() {
    ESP_LOGI(TAG, "Initializing PowerModule");

    #if defined(HAS_AXP192)
        if (axp.begin(Wire, AXP192_SLAVE_ADDRESS) == AXP_FAIL) {
            ESP_LOGE(TAG, "AXP192 init failed");
            return false;
        } else {
            ESP_LOGD(TAG, "DCDC1: %s", axp.isDCDC1Enable() ? "ENABLE" : "DISABLE");
            ESP_LOGD(TAG, "DCDC2: %s", axp.isDCDC2Enable() ? "ENABLE" : "DISABLE");
            ESP_LOGD(TAG, "LDO2: %s", axp.isLDO2Enable() ? "ENABLE" : "DISABLE");
            ESP_LOGD(TAG, "LDO3: %s", axp.isLDO3Enable() ? "ENABLE" : "DISABLE");
            ESP_LOGD(TAG, "DCDC3: %s", axp.isDCDC3Enable() ? "ENABLE" : "DISABLE");
            ESP_LOGD(TAG, "Exten: %s", axp.isExtenEnable() ? "ENABLE" : "DISABLE");
        }
        return true;
    #elif
        return false;
    #endif
}

void PowerModule::gpsOn() {
    #if defined(HAS_AXP192) && defined(HAS_GPS)
        ESP_LOGD(TAG, "Turning on power for GPS");
        axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
    #endif
}

void PowerModule::gpsOff() {
    #if defined(HAS_AXP192) && defined(HAS_GPS)
        ESP_LOGD(TAG, "Turning off power for GPS");
        axp.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
    #endif
}
