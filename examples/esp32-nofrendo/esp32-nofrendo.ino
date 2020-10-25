#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include <SD.h>
#include <SPIFFS.h>

#include "hw_config.h"

extern "C"
{
#include <nofrendo.h>
}

void setup()
{
    Serial.begin(115200);

    // turn off WiFi
    esp_wifi_deinit();

    // disable Core 0 WDT
    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    // uncomment one for selecting files stored in SPIFFS or SD
    SPIFFS.begin(false, FSROOT);
    // SD.begin(SS, SPI, 40000000, FSROOT);

    Serial.println("NoFrendo start!\n");
    nofrendo_main(0, NULL);
    Serial.println("NoFrendo end!\n");
}

void loop()
{
}
