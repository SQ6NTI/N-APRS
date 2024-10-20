#include "configuration.h"
#include "defaults.h"
#include "modules/Modules.h"

/* TODO: Add remaining definitions */
I2CModule* i2cModule;
PowerModule* powerModule;
StateModule* stateModule;
GPSModule* gpsModule;
RadioModule* radioModule;
APRSModule* aprsModule;

void deployModules(Scheduler* aScheduler) {
    i2cModule = new I2CModule();
    powerModule = new PowerModule();
    stateModule = new StateModule();
    gpsModule = new GPSModule();
    radioModule = new RadioModule(aScheduler);
    aprsModule = new APRSModule();
}
