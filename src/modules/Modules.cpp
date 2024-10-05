#include "configuration.h"
#include "defaults.h"
#include "modules/Modules.h"


void deployModules() {
    i2cModule = new I2CModule();
    powerModule = new PowerModule();
    stateModule = new StateModule();
    gpsModule = new GPSModule();
}
