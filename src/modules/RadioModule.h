#pragma once

#include <queue>
#include <SPI.h>
#include <esp_log.h>
#include <RadioLib.h>
#include <TaskSchedulerDeclarations.h>
#include "modules/SPIModule.h"

class RadioModule {
    public:
        /* Constructor requires Scheduler to be specified */
        RadioModule(Scheduler* aScheduler);
        
        /* This should be called first after creating new RadioModule object */
        bool initialize();

        /* Instructs the radio to start receiving data */
        bool startReceive();

        /* TODO: Instructs the radio to start receiving data with specified IRQ flags
         * Specific to SX126x
        */
        //bool startReceive(RadioLibIrqFlags_t irqFlags, RadioLibIrqFlags_t irqMask);

        /* This is a workaround to a problem of attaching class member as interrupt handler INSIDE the class itself.
        * https://forum.arduino.cc/t/interrupt-in-class-esp32/1039326/25
        */
        void attachInterrupt(void (*callback)());

        /* Interrupt Service Routine (ISR) is kept in IRAM to avoid calling FLASH during interrupt handling.
        * Use inside non-capturing lambda, because this is not static.
        */
        void interruptHandler(void);

        /* This is needed by APRSClient */
        PhysicalLayer* getRadioInterface();

        class Receiver : public Task {
            public:
                Receiver(Scheduler* aScheduler, RadioModule* radioModule);
                bool Callback();
                String receivePacket();
            private:
                RadioModule* radioModule;
        } tReceiver;

        /* TODO: RX 
        packet queue */
    protected:
        bool receiving = false;
        bool transmitting = false;
    private:
        Scheduler* aScheduler;

        RadioLibHal* hal = new ArduinoHal(spiModule->getRadioSPI(), spiModule->getRadioSPISettings());

        #if defined(HAS_SX1268)
            SX1268 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #elif defined(HAS_SX1262)
            SX1262 radioInterface = new Module(hal, SX126X_CS, SX126X_IRQ, SX126X_RESET, SX126X_BUSY);
        #endif
        
};

class RM2 : RadioModule {

};