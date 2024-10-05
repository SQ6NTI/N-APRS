#pragma once

#include <esp_log.h>

class RadioModule {
    public:
        RadioModule();
        bool initialize();
    protected:
    private:
};

extern RadioModule *radioModule;
