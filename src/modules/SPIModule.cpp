#include "configuration.h"
#include "defaults.h"
#include "util.h"
#include "SPIModule.h"

static const char *const TAG = "SPIModule";

SPIModule *spiModule;

SPIModule::SPIModule() {
}

SPIClass& SPIModule::getRadioSPI() {
    return SPI;
}

SPISettings SPIModule::getRadioSPISettings() {
    SPISettings spiSettings(RADIO_SPI_FREQ, SPI_MSBFIRST, SPI_MODE0);
    return spiSettings;
}

void SPIModule::initializeRadioSPI() {
    ESP_LOGI(TAG, "Starting SPI (SCK: %d, MISO: %d, MOSI: %d, CS: %d)",
        RADIO_SCK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_CS_PIN);
    
    SPI.begin(RADIO_SCK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_CS_PIN);
    SPI.setFrequency(RADIO_SPI_FREQ);
}
