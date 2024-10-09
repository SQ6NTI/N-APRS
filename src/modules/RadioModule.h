#pragma once

#include <RadioLib.h>

#if defined(HAS_SX126X)
    #if defined(HAS_SX1268)
        SX1268 radioModule = new Module(SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
    #endif
#endif

class RadioModule {
    public:
        RadioModule();
        bool initialize();
        
    protected:
    private:
};

extern RadioModule *radioModule;
