#include "configuration.h"
#include "defaults.h"
#include "modules/Modules.h"

I2CModule* i2cModule;
PowerModule* powerModule;
/* FIXME: might not need this StateModule eventually */
//StateModule* stateModule;
GNSSModule* gnssModule;
RadioModule* radioModule;
APRSModule* aprsModule;

void deployModules(Scheduler* aScheduler) {
    i2cModule = new I2CModule();
    powerModule = new PowerModule();
    /* FIXME: might not need this StateModule eventually */
    //stateModule = new StateModule();
    gnssModule = new GNSSModule(aScheduler);
    radioModule = new RadioModule(aScheduler);
    aprsModule = new APRSModule();
}
