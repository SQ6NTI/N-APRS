#include "configuration.h"
#include "defaults.h"
#include "Tasks.h"

namespace multitasking {
    /* Logging tag for this class */
    static const char *const TAG = "Tasks";

    Tasks::Tasks() {
        tRadioRx = new Task(TASK_IMMEDIATE, TASK_FOREVER, NULL, &taskScheduler, false);
    }

    void Tasks::radioDataRx() {
        tRadioRx->enable();
    }

}