#pragma once

#include <esp_log.h>
#if defined(HAS_UBLOX)
    #include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#endif
#include <TaskSchedulerDeclarations.h>

#if defined(HAS_PMU)
    #include "modules/PowerModule.h"
#endif
#include "GNSSPosition.pb.h"

class GNSSModule {
    public:
        /* Constructor requires Scheduler to be specified */
        GNSSModule(Scheduler* aScheduler);

        class Position : public Task {
            public:
                Receiver(Scheduler* aScheduler, RadioModule* radioModule);
                bool Callback();
                bool OnEnable();
                void OnDisable();
            private:
                RadioModule* radioModule;
                LoRaPacket loraPacket;
        } tReceiver;
        
        static HardwareSerial gnssSerial;

        #if defined(HAS_PMU)
            bool initialize(PowerModule* powerModule);
        #endif
        bool initialize();
        void checkPosition();
    protected:
        GNSSPosition currentPosition;
    private:
        Scheduler* aScheduler;
        #if defined(HAS_PMU)
            PowerModule* powerModule;
        #endif
        #if defined(HAS_UBLOX)
            SFE_UBLOX_GNSS uBloxGNSS;
        #endif
};
