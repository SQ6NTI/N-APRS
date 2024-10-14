#pragma once

#include <SPI.h>
#include <esp_log.h>
#include <RadioLib.h>
#include <TaskSchedulerDeclarations.h>
#include "modules/SPIModule.h"

class RadioModule {
    public:
        /*class Receiver : Task {

        };*/
        RadioModule(Scheduler aScheduler);
        static RadioModule *instance;
        
        bool Callback();
        bool initialize();
        void attachInterrupt(void (*callback)());
        //void beginTask();
        PhysicalLayer* getRadioInterface();
        void receivePacket();
        void (*rxDoneCallback)();
        void interruptHandler(void);
    protected:

        #if defined(HAS_SX1268)
            SX1268 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #elif defined(HAS_SX1262)
            SX1262 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #endif
        
        volatile bool interruptReceived = false;
    private:
        Scheduler aScheduler;
        Task tRadioRx;
        RadioLibHal *hal = new ArduinoHal(spiModule->getRadioSPI(), spiModule->getRadioSPISettings());
};
