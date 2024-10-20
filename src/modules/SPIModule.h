#pragma once

#include <esp_log.h>
#include <SPI.h>

class SPIModule {
    public:
        SPIModule();
        void initializeRadioSPI();
        SPIClass& getRadioSPI();
        SPISettings getRadioSPISettings();
    protected:
    private:
};

extern SPIModule *spiModule;
