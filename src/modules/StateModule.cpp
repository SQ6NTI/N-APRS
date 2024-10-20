#include "configuration.h"
#include "defaults.h"
#include "StateModule.h"

static const char *const TAG = "StateModule";

StateModule::StateModule() {

}

bool StateModule::initialize() {
    ESP_LOGI(TAG, "Initializing StateModule");
    
    #if defined(HAS_GPS)
        return gpsModule->initialize();
    #endif

    return true;
}

void StateModule::checkPosition() {
    #if defined(HAS_GPS)
        gpsModule->checkPosition();
    #endif

}