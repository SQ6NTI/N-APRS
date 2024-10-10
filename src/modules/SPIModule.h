#pragma once

#include <SPI.h>
#include <esp_log.h>

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
