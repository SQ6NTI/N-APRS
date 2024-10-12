#pragma once

#include <esp_log.h>
#include <TaskScheduler.h>

namespace multitasking {
    class Tasks {
        public:
            Scheduler taskScheduler;
            static Task *tRadioRx;
            
            static void radioDataRx();
        protected:

        private:

    };
}
