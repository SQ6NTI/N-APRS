#pragma once

#include <queue>
#include <SPI.h>
#include <esp_log.h>
#include <RadioLib.h>
#include <TaskSchedulerDeclarations.h>
#include "modules/SPIModule.h"

class RadioModule {
    public:
        RadioModule(Scheduler* aScheduler);
        
        bool initialize();
        void attachInterrupt(void (*callback)());
        PhysicalLayer* getRadioInterface();
        void interruptHandler(void);

        class Receiver : public Task {
            public:
                Receiver(Scheduler* aScheduler, RadioModule* radioModule);
                bool Callback();
                String receivePacket();
        } tReceiver;

        /* TODO: RX 
        packet queue */

        static RadioModule* instance;
    protected:
        #if defined(HAS_SX1268)
            SX1268 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #elif defined(HAS_SX1262)
            SX1262 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #endif
        volatile bool interruptReceived = false;
    private:
        Scheduler* aScheduler;
        RadioLibHal* hal = new ArduinoHal(spiModule->getRadioSPI(), spiModule->getRadioSPISettings());
};
