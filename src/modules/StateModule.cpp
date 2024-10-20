#include "configuration.h"
#include "defaults.h"
#include "StateModule.h"

static const char *const TAG = "StateModule";

StateModule::StateModule() {

}

bool StateModule::initialize() {
    ESP_LOGI(TAG, "Initializing StateModule");

    return true;
}
