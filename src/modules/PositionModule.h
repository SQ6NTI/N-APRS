#pragma once
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

class PositionModule {
    public:
        PositionModule();
};

extern PositionModule *positionModule;
