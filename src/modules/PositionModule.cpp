#include "configuration.h"
#include "defaults.h"
#include "PositionModule.h"

#if defined(HAS_AXP192)
  AXP20X_Class axp;
#endif

HardwareSerial PositionModule::gpsSerial(1);

PositionModule *positionModule;

PositionModule::PositionModule() {

}

bool PositionModule::initialize() {
    Serial.println(F("Initializing AXP192"));
    if (axp.begin(Wire, AXP192_SLAVE_ADDRESS) == AXP_FAIL) {
        Serial.println(F("AXP192 init failed"));
    } else {
        Serial.printf("DCDC1: %s\n", axp.isDCDC1Enable() ? "ENABLE" : "DISABLE");
        Serial.printf("DCDC2: %s\n", axp.isDCDC2Enable() ? "ENABLE" : "DISABLE");
        Serial.printf("LDO2: %s\n", axp.isLDO2Enable() ? "ENABLE" : "DISABLE");
        Serial.printf("LDO3: %s\n", axp.isLDO3Enable() ? "ENABLE" : "DISABLE");
        Serial.printf("DCDC3: %s\n", axp.isDCDC3Enable() ? "ENABLE" : "DISABLE");
        Serial.printf("Exten: %s\n", axp.isExtenEnable() ? "ENABLE" : "DISABLE");
        Serial.println("-------------------------------------");
    }

    Serial.println(F("Turning on power for GPS"));
    gpsOn();

    Serial.println(F("Initializing serial for GPS"));
    gpsSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    Serial.println(F("Initializing GPS"));
    //myGNSS.enableDebugging();
    if (!myGNSS.begin(gpsSerial)) {
        Serial.println(F("u-blox GNSS failed to start"));
        while (1);
    }

    Serial.println(F("Setting UART1 output to NMEA"));
    myGNSS.setUART1Output(COM_TYPE_NMEA);
    Serial.println(F("Piping messages to Serial"));
    myGNSS.setNMEAOutputPort(Serial);

    return true;
}

void PositionModule::gpsOff() {
    #if defined(HAS_AXP192)
        axp.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
    #endif
}

void PositionModule::gpsOn() {
    #if defined(HAS_AXP192)
        axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
    #endif
}

void PositionModule::checkPosition() {
    //myGNSS.checkUblox();
    long latitude = myGNSS.getLatitude();
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGNSS.getLongitude();
    Serial.print(F(" Long: "));
    Serial.print(longitude);
    Serial.print(F(" (degrees * 10^-7)"));

    long altitude = myGNSS.getAltitude();
    Serial.print(F(" Alt: "));
    Serial.print(altitude);
    Serial.print(F(" (mm)"));

    byte SIV = myGNSS.getSIV();
    Serial.print(F(" SIV: "));
    Serial.print(SIV);

    Serial.println();
}
