#pragma once

#include <TaskSchedulerDeclarations.h>

#include "modules/I2CModule.h"
#include "modules/PowerModule.h"
#include "modules/StateModule.h"
#include "modules/GPSModule.h"
#include "modules/RadioModule.h"
#include "modules/APRSModule.h"

/* TODO: Add remaining externs */
extern I2CModule* i2cModule;
extern PowerModule *powerModule;
extern StateModule *stateModule;
extern GPSModule* gpsModule;
extern RadioModule* radioModule;
extern APRSModule* aprsModule;

void deployModules(Scheduler* aScheduler);
