#include "configuration.h"
#include "defaults.h"
#include "modules/GNSSModule.h"

static const char *const TAG = "GNSSModule";

HardwareSerial GNSSModule::gnssSerial(1);

GNSSModule::GNSSModule(Scheduler* aScheduler) {
    this->aScheduler = aScheduler;
    currentPosition = GNSSPosition_init_default;
}

#if defined(HAS_PMU)
bool GNSSModule::initialize(PowerModule* powerModule) {
    this->powerModule = powerModule;
    this->powerModule->gnssOn();
    return initialize();
}
#endif

bool GNSSModule::initialize() {
    ESP_LOGD(TAG, "Initializing serial for GNSS");
    gnssSerial.begin(GNSS_BAUD_RATE, SERIAL_8N1, GNSS_RX_PIN, GNSS_TX_PIN);

    ESP_LOGD(TAG, "Initializing GNSS");
    #if defined(HAS_UBLOX)
        //uBloxGNSS.enableDebugging();
        if (!uBloxGNSS.begin(gnssSerial)) {
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

void GNSSModule::checkPosition() {
    /* Below example 3D fix data is for testing */
    currentPosition.latitude = 510864341; /* 51.0864341 deg N */
    currentPosition.longitude = 170156686; /* 17.0156686 deg E */
    currentPosition.altitude = 168836; /* 168.836 m */
    currentPosition.time = 1728160247; /* Sat Oct 05 2024 20:30:47 GMT+0000 */
    currentPosition.pdop = 1236; /* PDOP = 12.36 */
    currentPosition.hdop = 650; /* HDOP = 6.5 */
    currentPosition.vdop = 1052; /* VDOP = 10.52 */
    currentPosition.ground_speed = 1425; /* 1.425 m */
    currentPosition.heading = 9492034; /* 94.92034 deg */
    currentPosition.fix_type = 3; /* 3D fix */
    currentPosition.siv = 4; /* 4 satellites in view */
    currentPosition.sequence_counter = 2103; /* Sequence number 2103 */

    /* Below example is 2D fix */
    /*
    [6213027][I][GNSSModule.cpp:59] checkPosition(): [GNSSModule] Lat : 510867526
    [6213033][I][GNSSModule.cpp:60] checkPosition(): [GNSSModule] Lon : 170155898
    [6213040][I][GNSSModule.cpp:61] checkPosition(): [GNSSModule] Alt : 191225
    [6213047][I][GNSSModule.cpp:62] checkPosition(): [GNSSModule] Unix: 1728160151
    [6213053][I][GNSSModule.cpp:63] checkPosition(): [GNSSModule] PDOP: 735
    [6213060][I][GNSSModule.cpp:64] checkPosition(): [GNSSModule] HDOP: 729
    [6213066][I][GNSSModule.cpp:65] checkPosition(): [GNSSModule] VDOP: 100
    [6213072][I][GNSSModule.cpp:66] checkPosition(): [GNSSModule] Gspd: 865
    [6213078][I][GNSSModule.cpp:67] checkPosition(): [GNSSModule] Head: 10162088
    [6213085][I][GNSSModule.cpp:68] checkPosition(): [GNSSModule] FixT: 2
    [6213091][I][GNSSModule.cpp:69] checkPosition(): [GNSSModule] SIV : 3
    [6213097][I][GNSSModule.cpp:70] checkPosition(): [GNSSModule] SEQ : 2071
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
    ESP_LOGD(TAG, "PDOP: %d", currentPosition.pdop);
    ESP_LOGD(TAG, "HDOP: %d", currentPosition.hdop);
    ESP_LOGD(TAG, "VDOP: %d", currentPosition.vdop);
    ESP_LOGD(TAG, "Gspd: %d", currentPosition.ground_speed);
    ESP_LOGD(TAG, "Head: %d", currentPosition.heading);
    ESP_LOGD(TAG, "FixT: %d", currentPosition.fix_type);
    ESP_LOGD(TAG, "SIV : %d", currentPosition.siv);
    ESP_LOGD(TAG, "SEQ : %d", currentPosition.sequence_counter);
}
