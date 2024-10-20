#pragma once

#include <esp_log.h>
#include <Wire.h>
#include <axp20x.h>

class PowerModule {
    public:
        PowerModule();
        bool initialize();
        void gnssOn();
        void gnssOff();
    protected:
    private:
};
