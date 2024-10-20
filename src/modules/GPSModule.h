#pragma once

#include <esp_log.h>

#if defined(HAS_PMU)
    #include <modules/PowerModule.h>
#endif

#if defined(HAS_UBLOX)
    #include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#endif

#include "Position.pb.h"

class GPSModule {
    public:
        static HardwareSerial gpsSerial;
        GPSModule();
        bool initialize();
        void checkPosition();
    protected:
        Position currentPosition;
    private:
        #if defined(HAS_UBLOX)
            SFE_UBLOX_GNSS uBloxGNSS;
        #endif
};

extern GPSModule *gpsModule;
