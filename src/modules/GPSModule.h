#pragma once

#include <esp_log.h>
#if defined(HAS_UBLOX)
    #include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#endif

#if defined(HAS_PMU)
    #include "modules/PowerModule.h"
#endif
#include "Position.pb.h"

class GPSModule {
    public:
        static HardwareSerial gpsSerial;
        GPSModule();
        #if defined(HAS_PMU)
            bool initialize(PowerModule* powerModule);
        #endif
        bool initialize();
        void checkPosition();
    protected:
        Position currentPosition;
    private:
        #if defined(HAS_PMU)
            PowerModule* powerModule;
        #endif
        #if defined(HAS_UBLOX)
            SFE_UBLOX_GNSS uBloxGNSS;
        #endif
};
