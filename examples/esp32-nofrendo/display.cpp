extern "C"
{
#include <nes/nes.h>
}

#include "hw_config.h"

#include <Arduino_GFX_Library.h>
#define TFT_BRIGHTNESS 128 /* 0 - 255 */

/* M5Stack */
#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)

#define TFT_BL 32
Arduino_ESP32SPI_DMA *bus = new Arduino_ESP32SPI_DMA(27 /* DC */, 14 /* CS */, SCK, MOSI, MISO);
Arduino_ILI9341_M5STACK *gfx = new Arduino_ILI9341_M5STACK(bus, 33 /* RST */, 1 /* rotation */);

/* Odroid-Go */
#elif defined(ARDUINO_ODROID_ESP32)

#define TFT_BL 14
Arduino_ESP32SPI_DMA *bus = new Arduino_ESP32SPI_DMA(21 /* DC */, 5 /* CS */, SCK, MOSI, MISO);
// Arduino_ILI9341 *gfx = new Arduino_ILI9341(bus, -1 /* RST */, 3 /* rotation */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, -1 /* RST */, 1 /* rotation */, true /* IPS */);

/* TTGO T-Watch */
#elif defined(ARDUINO_T) || defined(ARDUINO_TWATCH_BASE) || defined(ARDUINO_TWATCH_2020_V1) || defined(ARDUINO_TWATCH_2020_V2) // TTGO T-Watch

#define TFT_BL 12
Arduino_DataBus *bus = new Arduino_ESP32SPI_DMA(27 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, -1 /* MISO */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, -1 /* RST */, 1 /* rotation */, true /* IPS */, 240, 240, 0, 80);

/* custom hardware */
#else

#define TFT_BL 32
Arduino_ESP32SPI_DMA *bus = new Arduino_ESP32SPI_DMA(27 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, 33 /* RST */, 3 /* rotation */, true /* IPS */);

// #define TFT_BL 23
// Arduino_ESP32SPI_DMA *bus = new Arduino_ESP32SPI_DMA(19 /* DC */, 5 /* CS */, 22 /* SCK */, 21 /* MOSI */, -1 /* MISO */);
// Arduino_ST7796 *gfx = new Arduino_ST7796(bus, 18, 1 /* rotation */);

#endif /* custom hardware */

static int16_t w, h, frame_x, frame_y, frame_x_offset, frame_width, frame_height, frame_line_pixels;
static int16_t bg_color;
extern uint16_t myPalette[];

extern "C" void display_init()
{
    gfx->begin();
    bg_color = gfx->color565(24, 28, 24); // DARK DARK GREY
    gfx->fillScreen(bg_color);

#ifdef TFT_BL
    ledcAttachPin(TFT_BL, 1);     // assign TFT_BL pin to channel 1
    ledcSetup(1, 12000, 8);       // 12 kHz PWM, 8-bit resolution
    ledcWrite(1, TFT_BRIGHTNESS); // brightness 0 - 255
#endif

    w = gfx->width();
    h = gfx->height();
    if (w < 480) // assume only 240x240 or 320x240
    {
        if (w > NES_SCREEN_WIDTH)
        {
            frame_x = (w - NES_SCREEN_WIDTH) / 2;
            frame_x_offset = 0;
            frame_width = NES_SCREEN_WIDTH;
            frame_height = NES_SCREEN_HEIGHT;
            frame_line_pixels = frame_width;
        }
        else
        {
            frame_x = 0;
            frame_x_offset = (NES_SCREEN_WIDTH - frame_width) / 2;
            frame_width = w;
            frame_height = NES_SCREEN_HEIGHT;
            frame_line_pixels = frame_width;
        }
        frame_y = (gfx->height() - NES_SCREEN_HEIGHT) / 2;
    }
    else // assume 480x320
    {
        frame_x = 0;
        frame_y = 0;
        frame_x_offset = 8;
        frame_width = w;
        frame_height = h;
        frame_line_pixels = frame_width / 2;
    }
}

extern "C" void display_write_frame(const uint8_t *data[])
{
    gfx->startWrite();
    if (w < 480)
    {
        gfx->writeAddrWindow(frame_x, frame_y, frame_width, frame_height);
        for (int32_t i = 0; i < NES_SCREEN_HEIGHT; i++)
        {
            gfx->writeIndexedPixels((uint8_t *)(data[i] + frame_x_offset), myPalette, frame_line_pixels);
        }
    }
    else
    {
        // ST7796 480 x 320 resolution
        // crop 256 x 240 to 240 x 214
        // then scale up width x 2 and scale up height x 1.5

        // scale up height x 1.5 have 2 options
        // Option 1: repeat a line for every 2 lines
        gfx->writeAddrWindow(frame_x, frame_y, frame_width, frame_height);
        for (int16_t i = 10; i < (10 + 214); i++)
        {
            gfx->writeIndexedPixelsDouble((uint8_t *)(data[i] + 8), myPalette, frame_line_pixels);
            if ((i % 2) == 1)
            {
                gfx->writeIndexedPixelsDouble((uint8_t *)(data[i] + 8), myPalette, frame_line_pixels);
            }
        }
        // Option 2: simply blank a line for every 2 lines
        int16_t y = 0;
        for (int16_t i = 10; i < (10 + 214); i++)
        {
            gfx->writeAddrWindow(frame_x, y++, frame_width, 1);
            gfx->writeIndexedPixelsDouble((uint8_t *)(data[i] + 8), myPalette, frame_line_pixels);
            if ((i % 2) == 1)
            {
                y++; // blank line
            }
        }
    }
    gfx->endWrite();
}

extern "C" void display_clear()
{
    gfx->fillScreen(bg_color);
}
