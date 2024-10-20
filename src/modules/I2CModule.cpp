#include "configuration.h"
#include "defaults.h"
#include "util.h"
#include "I2CModule.h"

static const char *const TAG = "I2CModule";

I2CModule::I2CModule() {

}

bool I2CModule::initialize() {
    ESP_LOGI(TAG, "Starting I2C (SDA: %s, SCL: %s)", STR(I2C_SDA), STR(I2C_SCL));
    return Wire.begin(I2C_SDA, I2C_SCL);
}
