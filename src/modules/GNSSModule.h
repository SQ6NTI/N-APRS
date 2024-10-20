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

        #if defined(HAS_PMU)
            /* Constructor that allows GNSSModule to turn on power for GNSS device */
            bool initialize(PowerModule* powerModule);
        #endif
        /* This should be called first after creating new GNSSModule object */
        bool initialize();

        /* This is likely a temporary method of manual position checking */
        void checkPosition();

        /* Wrapper to read currentPosition */
        GNSSPosition getCurrentPosition();

        class Receiver : public Task {
            public:
                /* Constructor for the GNSSModule's nested class Receiver */
                Receiver(Scheduler* aScheduler, GNSSModule* gnssModule);

                /* Handler for receiving location from the connected GNSS device */
                bool Callback();
                bool OnEnable();
                void OnDisable();
            private:
                GNSSModule* gnssModule;
        } tReceiver;
    protected:
        GNSSPosition currentPosition;
    private:
        Scheduler* aScheduler;

        static HardwareSerial gnssSerial;

        #if defined(HAS_PMU)
            PowerModule* powerModule;
        #endif
        #if defined(HAS_UBLOX)
            SFE_UBLOX_GNSS uBloxGNSS;
        #endif
};
