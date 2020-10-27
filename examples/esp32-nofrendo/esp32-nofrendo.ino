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
    FS fs = SPIFFS;
    // SD.begin(SS, SPI, 40000000, FSROOT);

    // find first rom file (*.nes)
    File root = fs.open("/");
    if (!root)
    {
        Serial.println("Failed to open directory");
    }
    else
    {
        bool foundRom = false;
        char *argv[1];

        File file = root.openNextFile();
        while (file)
        {
            if (file.isDirectory())
            {
                // skip
            }
            else
            {
                char *filename = (char *)file.name();
                int8_t len = strlen(filename);
                if (strstr(strlwr(filename + (len - 4)), ".nes"))
                {
                    foundRom = true;
                    char fullFilename[256];
                    sprintf(fullFilename, "%s%s", FSROOT, filename);
                    Serial.println(fullFilename);
                    argv[0] = fullFilename;
                    break;
                }
            }

            file = root.openNextFile();
        }

        if (foundRom)
        {
            Serial.println("NoFrendo start!\n");
            nofrendo_main(1, argv);
            Serial.println("NoFrendo end!\n");
        }
        else
        {
            Serial.println("Failed to find rom file, please copy rom file to data folder and upload with \"ESP32 Sketch Data Upload\"");
        }
    }
}

void loop()
{
}
