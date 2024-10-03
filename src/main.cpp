#include <Arduino.h>
#include <Wire.h>

#include "configuration.h"
#include "modules/PositionModule.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

unsigned long lastGNSSsend = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("[[[ ESP32-LoRa-APRS ]]]"));
    Serial.println(F(""));
    Serial.println(F(""));

    #if defined(HAS_I2C)
        Serial.println((String)"Starting I2C (SDA: "+STR(I2C_SDA)+", SCL: "+STR(I2C_SCL)+")");
        Wire.begin(I2C_SDA, I2C_SCL);
    #endif

    #if defined(HAS_GPS)
        Serial.println(F("Initializing PositionModule"));
        positionModule = new PositionModule();
        positionModule->initialize();
    #endif
    
    Serial.println(F("Initialization complete"));
}

void loop() {
    if (millis() - lastGNSSsend > 1000) {
        #if defined(HAS_GPS)
            positionModule->checkPosition();
        #endif
        lastGNSSsend = millis();
    }
}
