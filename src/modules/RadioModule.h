#pragma once

#include <SPI.h>
#include <RadioLib.h>
#include <esp_log.h>
#include "modules/SPIModule.h"

class RadioModule {
    public:
        RadioModule();
        bool initialize();
        PhysicalLayer* getRadioInterface();
    protected:
        #if defined(HAS_SX1268)
            SX1268 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #elif defined(HAS_SX1262)
            SX1262 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #endif
    private:
        RadioLibHal *hal = new ArduinoHal(spiModule->getRadioSPI(), spiModule->getRadioSPISettings());
};

extern RadioModule *radioModule;
