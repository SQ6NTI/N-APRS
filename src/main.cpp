/*
 * N-APRS
 * Modular APRS software for Arduino
 * Source: https://github.com/SQ6NTI/N-APRS
*/

#include <Arduino.h>
#include <esp_log.h>
#include <TaskScheduler.h>

#include "configuration.h"
#include "util.h"
#include "modules/Modules.h"

static const char* TAG = "main";
static const char* VERSION = "0.0.1";
static const char* VERSION_DATE = "2024-10-21";

Scheduler taskScheduler;

/* TODO: remove these temp variables later on */
unsigned long lastGNSSsend = 0;
unsigned long lastAPRSsend = 0;
APRSClient* aprs;

void setup() {
    /* Creates module class instances */
    deployModules(&taskScheduler);

    Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.printf("---------- N-APRS %s (%s)----------\r\n", VERSION, VERSION_DATE);
    Serial.println();
    Serial.println();
    Serial.println("System starting...");
    

    #if defined(HAS_I2C)
        i2cModule->initialize();
    #endif

    #if defined(HAS_PMU)
        powerModule->initialize();
    #endif

    radioModule->initialize();
    radioModule->attachInterrupt([]() { radioModule->interruptHandler(); });
    radioModule->startReceive();
    
    LoRaPacket loraPacket = LoRaPacket_init_default;
    std::string aprs_data = RADIOLIB_APRS_LORA_HEADER;
    aprs_data += "SQ6NTI-5>APLN01:@093045z5105.20N/01700.90E>test tracker";
    memcpy(loraPacket.data.bytes, (uint8_t*)aprs_data.c_str(), aprs_data.length());
    loraPacket.data.size = aprs_data.length();
    radioModule->tTransmitter.setPacket(loraPacket);

    gnssModule->initialize();
    
    aprs = new APRSClient(radioModule->getRadioInterface());
    char source[] = "SQ6NTI";
    int aprsState = aprs->begin('>', source, 5);
    ESP_LOGD(TAG, "APRS Client initialization state: %d", aprsState);

    ESP_LOGI(TAG, "Initialization complete");
}

void loop() {
    /*if (millis() - lastGNSSsend > 1000) {
        stateModule->checkPosition();
        lastGNSSsend = millis();
    }*/

    taskScheduler.execute();

    if (millis() - lastAPRSsend > 60000) {
        radioModule->tTransmitter.restart();
        lastAPRSsend = millis();
    }

    /*if (millis() - lastAPRSsend > 60000) {
        ESP_LOGD(TAG, "Sending APRS position");
        char destination[] = "APLN01";
        char latitude[] = "5105.20N";
        char longitude[] = "01700.90E";
        char message[] = "test tracker";
        char timestamp[] = "093045z";
        int state = aprs->sendPosition(destination, 1, latitude, longitude, message, timestamp);
        ESP_LOGD(TAG, "APRS position sending state: %d", state);
        lastAPRSsend = millis();
    }*/
}
