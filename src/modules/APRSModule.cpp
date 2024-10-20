#include "configuration.h"
#include "defaults.h"
#include "APRSModule.h"

static const char *const TAG = "APRSModule";

APRSModule::APRSModule() {

}

bool APRSModule::initialize() {
    ESP_LOGI(TAG, "Initializing APRSModule");

    return true;
}
