#include <Arduino.h>
#include <esp_log.h>

#include "configuration.h"
#include "util.h"
#include "modules/Modules.h"

unsigned long lastGNSSsend = 0;
static const char* TAG = "main";

void setup() {
    // Creates module class instances
    deployModules();

    // TODO: temporary reference to serial for debugging aid
    Serial.begin(115200);

    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "---------- ESP32-LoRa-APRS ----------");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "System starting");

    #if defined(HAS_I2C)
        i2cModule->initialize();
    #endif

    #if defined(HAS_PMU)
        powerModule->initialize();
    #endif

    stateModule->initialize();
    
    ESP_LOGI(TAG, "Initialization complete");
}

void loop() {
    if (millis() - lastGNSSsend > 1000) {
        stateModule->checkPosition();
        lastGNSSsend = millis();
    }
}
