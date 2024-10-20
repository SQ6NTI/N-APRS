#include "configuration.h"
#include "defaults.h"
#include "modules/GNSSModule.h"

static const char *const TAG = "GNSSModule";

HardwareSerial GNSSModule::gnssSerial(1);

GNSSModule::GNSSModule(Scheduler* aScheduler)
        : tReceiver(aScheduler, this) {
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

GNSSPosition GNSSModule::getCurrentPosition() {
    return currentPosition;
}

GNSSModule::Receiver::Receiver(Scheduler* aScheduler, GNSSModule* gnssModule)
        : Task(TASK_SECOND, TASK_FOREVER, aScheduler, false) {
    this->gnssModule = gnssModule;
    this->gnssModule->currentPosition = GNSSPosition_init_default;
}

bool GNSSModule::Receiver::Callback() {
    /* TODO: implement better */
    /* Below example 3D fix data is for testing */
    gnssModule->currentPosition.latitude = 510864341; /* 51.0864341 deg N */
    gnssModule->currentPosition.longitude = 170156686; /* 17.0156686 deg E */
    gnssModule->currentPosition.altitude = 168836; /* 168.836 m */
    gnssModule->currentPosition.time = 1728160247; /* Sat Oct 05 2024 20:30:47 GMT+0000 */
    gnssModule->currentPosition.pdop = 1236; /* PDOP = 12.36 */
    gnssModule->currentPosition.hdop = 650; /* HDOP = 6.5 */
    gnssModule->currentPosition.vdop = 1052; /* VDOP = 10.52 */
    gnssModule->currentPosition.ground_speed = 1425; /* 1.425 m */
    gnssModule->currentPosition.heading = 9492034; /* 94.92034 deg */
    gnssModule->currentPosition.fix_type = 3; /* 3D fix */
    gnssModule->currentPosition.siv = 4; /* 4 satellites in view */
    gnssModule->currentPosition.sequence_counter = 2103; /* Sequence number 2103 */

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
        gnssModule->currentPosition.latitude = uBloxGNSS.getLatitude();
        gnssModule->currentPosition.longitude = uBloxGNSS.getLongitude();
        gnssModule->currentPosition.altitude = uBloxGNSS.getAltitudeMSL();
        gnssModule->currentPosition.time = uBloxGNSS.getUnixEpoch();
        gnssModule->currentPosition.position_source = Position_PositionSource_POSITION_INTERNAL;
        gnssModule->currentPosition.altitude_source = Position_AltitudeSource_ALTITUDE_MANUAL;
        gnssModule->currentPosition.PDOP = uBloxGNSS.getPDOP();
        gnssModule->currentPosition.HDOP = uBloxGNSS.getHorizontalDOP();
        gnssModule->currentPosition.VDOP = uBloxGNSS.getVerticalDOP();
        gnssModule->currentPosition.ground_speed = uBloxGNSS.getGroundSpeed();
        gnssModule->currentPosition.heading = uBloxGNSS.getHeading();
        gnssModule->currentPosition.fix_type = uBloxGNSS.getFixType();
        gnssModule->currentPosition.siv = uBloxGNSS.getSIV();
        gnssModule->currentPosition.sequence_counter++;
    #endif*/

    ESP_LOGD(TAG, "Lat: %d   Lon: %d   Alt: %d   Unix: %d   PDOP: %d   HDOP: %d   VDOP: %d   Gspd: %d   Head: %d   FixT: %d   SIV: %d   SEQ : %d",
        gnssModule->currentPosition.latitude,
        gnssModule->currentPosition.longitude,
        gnssModule->currentPosition.altitude,
        gnssModule->currentPosition.time,
        gnssModule->currentPosition.pdop,
        gnssModule->currentPosition.hdop,
        gnssModule->currentPosition.vdop,
        gnssModule->currentPosition.ground_speed,
        gnssModule->currentPosition.heading,
        gnssModule->currentPosition.fix_type,
        gnssModule->currentPosition.siv,
        gnssModule->currentPosition.sequence_counter);

    return true;
}

bool GNSSModule::Receiver::OnEnable() {
    ESP_LOGD(TAG, "Receiver enabled");
    return true;
}

void GNSSModule::Receiver::OnDisable() {
    ESP_LOGD(TAG, "Receiver disabled");
}
