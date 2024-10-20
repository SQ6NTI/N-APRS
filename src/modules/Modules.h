#pragma once

#include <TaskSchedulerDeclarations.h>

#include "modules/I2CModule.h"
#include "modules/PowerModule.h"
/* FIXME: might not need this StateModule eventually */
//#include "modules/StateModule.h"
#include "modules/GNSSModule.h"
#include "modules/RadioModule.h"
#include "modules/APRSModule.h"

/* TODO: Add remaining externs */
extern I2CModule* i2cModule;
extern PowerModule *powerModule;
/* FIXME: might not need this StateModule eventually */
//extern StateModule *stateModule;
extern GNSSModule* gnssModule;
extern RadioModule* radioModule;
extern APRSModule* aprsModule;

void deployModules(Scheduler* aScheduler);
