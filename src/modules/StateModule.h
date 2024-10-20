#pragma once

#include <esp_log.h>

#if defined(HAS_GPS)
    #include "modules/GPSModule.h"
#endif

class StateModule {
    public:
        StateModule();
        bool initialize();
        void checkPosition();
    protected:
    private:
    
};

extern StateModule *stateModule;
