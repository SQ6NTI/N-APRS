#pragma once

#include <TaskSchedulerDeclarations.h>

#include "modules/I2CModule.h"
#include "modules/PowerModule.h"
#include "modules/StateModule.h"
#include "modules/GPSModule.h"
#include "modules/RadioModule.h"

/* TODO: Add remaining externs */
extern RadioModule *radioModule;

void deployModules(Scheduler aScheduler);
