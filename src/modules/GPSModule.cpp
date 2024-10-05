#include "configuration.h"
#include "defaults.h"
#include "modules/GPSModule.h"

static const char *const TAG = "GPSModule";

HardwareSerial GPSModule::gpsSerial(1);

GPSModule *gpsModule;

GPSModule::GPSModule() {
    currentPosition = Position_init_default;
}

bool GPSModule::initialize() {
    #if defined(HAS_PMU)
        powerModule->gpsOn();
    #endif

    ESP_LOGD(TAG, "Initializing serial for GPS");
    gpsSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    ESP_LOGD(TAG, "Initializing GPS");
    #if defined(HAS_UBLOX)
        //uBloxGNSS.enableDebugging();
        if (!uBloxGNSS.begin(gpsSerial)) {
            ESP_LOGE(TAG, "u-blox GNSS failed to start");
            while (1);
        }
        ESP_LOGD(TAG, "Setting UART1 output to UBX");
        uBloxGNSS.setUART1Output(COM_TYPE_UBX);

        //ESP_LOGD(TAG, "Piping messages to Serial");
        //uBloxGNSS.setNMEAOutputPort(Serial);
    #endif

    return true;
}

void GPSModule::checkPosition() {
    /* Below example 3D fix data is for testing */
    currentPosition.latitude = 510864341; /* 51.0864341 deg N */
    currentPosition.longitude = 170156686; /* 17.0156686 deg E */
    currentPosition.altitude = 168836; /* 168.836 m */
    currentPosition.time = 1728160247; /* Sat Oct 05 2024 20:30:47 GMT+0000 */
    currentPosition.PDOP = 1236; /* PDOP = 12.36 */
    currentPosition.HDOP = 650; /* HDOP = 6.5 */
    currentPosition.VDOP = 1052; /* VDOP = 10.52 */
    currentPosition.ground_speed = 1425; /* 1.425 m */
    currentPosition.heading = 9492034; /* 94.92034 deg */
    currentPosition.fix_type = 3; /* 3D fix */
    currentPosition.siv = 4; /* 4 satellites in view */
    currentPosition.sequence_counter = 2103; /* Sequence number 2103 */

    /* Below example is 2D fix */
    /*
    [6213027][I][GPSModule.cpp:59] checkPosition(): [GPSModule] Lat : 510867526
    [6213033][I][GPSModule.cpp:60] checkPosition(): [GPSModule] Lon : 170155898
    [6213040][I][GPSModule.cpp:61] checkPosition(): [GPSModule] Alt : 191225
    [6213047][I][GPSModule.cpp:62] checkPosition(): [GPSModule] Unix: 1728160151
    [6213053][I][GPSModule.cpp:63] checkPosition(): [GPSModule] PDOP: 735
    [6213060][I][GPSModule.cpp:64] checkPosition(): [GPSModule] HDOP: 729
    [6213066][I][GPSModule.cpp:65] checkPosition(): [GPSModule] VDOP: 100
    [6213072][I][GPSModule.cpp:66] checkPosition(): [GPSModule] Gspd: 865
    [6213078][I][GPSModule.cpp:67] checkPosition(): [GPSModule] Head: 10162088
    [6213085][I][GPSModule.cpp:68] checkPosition(): [GPSModule] FixT: 2
    [6213091][I][GPSModule.cpp:69] checkPosition(): [GPSModule] SIV : 3
    [6213097][I][GPSModule.cpp:70] checkPosition(): [GPSModule] SEQ : 2071
    */

    /*#if defined(HAS_UBLOX)
        currentPosition.latitude = uBloxGNSS.getLatitude();
        currentPosition.longitude = uBloxGNSS.getLongitude();
        currentPosition.altitude = uBloxGNSS.getAltitudeMSL();
        currentPosition.time = uBloxGNSS.getUnixEpoch();
        currentPosition.position_source = Position_PositionSource_POSITION_INTERNAL;
        currentPosition.altitude_source = Position_AltitudeSource_ALTITUDE_MANUAL;
        currentPosition.PDOP = uBloxGNSS.getPDOP();
        currentPosition.HDOP = uBloxGNSS.getHorizontalDOP();
        currentPosition.VDOP = uBloxGNSS.getVerticalDOP();
        currentPosition.ground_speed = uBloxGNSS.getGroundSpeed();
        currentPosition.heading = uBloxGNSS.getHeading();
        currentPosition.fix_type = uBloxGNSS.getFixType();
        currentPosition.siv = uBloxGNSS.getSIV();
        currentPosition.sequence_counter++;
    #endif*/

    ESP_LOGD(TAG, "Lat : %d", currentPosition.latitude);
    ESP_LOGD(TAG, "Lon : %d", currentPosition.longitude);
    ESP_LOGD(TAG, "Alt : %d", currentPosition.altitude);
    ESP_LOGD(TAG, "Unix: %d", currentPosition.time);
    ESP_LOGD(TAG, "PDOP: %d", currentPosition.PDOP);
    ESP_LOGD(TAG, "HDOP: %d", currentPosition.HDOP);
    ESP_LOGD(TAG, "VDOP: %d", currentPosition.VDOP);
    ESP_LOGD(TAG, "Gspd: %d", currentPosition.ground_speed);
    ESP_LOGD(TAG, "Head: %d", currentPosition.heading);
    ESP_LOGD(TAG, "FixT: %d", currentPosition.fix_type);
    ESP_LOGD(TAG, "SIV : %d", currentPosition.siv);
    ESP_LOGD(TAG, "SEQ : %d", currentPosition.sequence_counter);
}
