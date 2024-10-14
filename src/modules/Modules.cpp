#include "configuration.h"
#include "defaults.h"
#include "modules/Modules.h"

/* TODO: Add remaining definitions */
RadioModule *radioModule;

void deployModules(Scheduler aScheduler) {
    i2cModule = new I2CModule();
    powerModule = new PowerModule();
    stateModule = new StateModule();
    gpsModule = new GPSModule();
    radioModule = new RadioModule(aScheduler);
}
