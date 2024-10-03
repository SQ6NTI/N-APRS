#pragma once

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <axp20x.h>

class PositionModule {
    public:
        static HardwareSerial gpsSerial;
        PositionModule();
        bool initialize();
        void checkPosition();
        void gpsOff();
        void gpsOn();
    protected:
    private:
        SFE_UBLOX_GNSS myGNSS;
};

extern PositionModule *positionModule;
