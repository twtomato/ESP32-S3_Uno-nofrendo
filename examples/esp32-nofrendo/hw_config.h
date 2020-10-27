#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

#define FSROOT "/fs"


/* M5Stack */
#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)

/* enable audio */
#define HW_AUDIO

/* controller is I2C M5Stack CardKB */
#define HW_CONTROLLER_I2C_M5CARDKB

/* SD card CS pin */
#define SS 4

/* Odroid-Go */
#elif defined(ARDUINO_ODROID_ESP32)

/* enable audio */
#define HW_AUDIO

/* controller is GPIO */
#define HW_CONTROLLER_GPIO
#define HW_CONTROLLER_GPIO_ANALOG_JOYSTICK
#define HW_CONTROLLER_GPIO_UP_DOWN 35
#define HW_CONTROLLER_GPIO_LEFT_RIGHT 34
#define HW_CONTROLLER_GPIO_X 13
#define HW_CONTROLLER_GPIO_Y 0
#define HW_CONTROLLER_GPIO_SELECT 27
#define HW_CONTROLLER_GPIO_START 39
#define HW_CONTROLLER_GPIO_A 32
#define HW_CONTROLLER_GPIO_B 33

/* TTGO T-Watch */
#elif defined(ARDUINO_T) || defined(ARDUINO_TWATCH_BASE) || defined(ARDUINO_TWATCH_2020_V1) || defined(ARDUINO_TWATCH_2020_V2) // TTGO T-Watch

/* no audio */

/* controller is GPIO */
#define HW_CONTROLLER_GPIO
#define HW_CONTROLLER_GPIO_ANALOG_JOYSTICK
#define HW_CONTROLLER_GPIO_UP_DOWN 34
#define HW_CONTROLLER_GPIO_LEFT_RIGHT 33
#define HW_CONTROLLER_GPIO_X 14
#define HW_CONTROLLER_GPIO_Y 26
#define HW_CONTROLLER_GPIO_SELECT 15
#define HW_CONTROLLER_GPIO_START 36
#define HW_CONTROLLER_GPIO_A 13
#define HW_CONTROLLER_GPIO_B 25

/* custom hardware */
#else

// enable audio
#define HW_AUDIO
#define HW_AUDIO_EXTDAC
#define HW_AUDIO_EXTDAC_BCLK 25
#define HW_AUDIO_EXTDAC_WCLK 26
#define HW_AUDIO_EXTDAC_DOUT 27

/* controller is GPIO */
#define HW_CONTROLLER_GPIO
#define HW_CONTROLLER_GPIO_ANALOG_JOYSTICK
#define HW_CONTROLLER_GPIO_UP_DOWN 35
#define HW_CONTROLLER_GPIO_LEFT_RIGHT 34
#define HW_CONTROLLER_GPIO_X 33
#define HW_CONTROLLER_GPIO_Y 32
#define HW_CONTROLLER_GPIO_SELECT 0
#define HW_CONTROLLER_GPIO_START 4
#define HW_CONTROLLER_GPIO_A 19
#define HW_CONTROLLER_GPIO_B 13

/* controller is I2C M5Stack CardKB */
// #define HW_CONTROLLER_I2C_M5CARDKB

/* controller is I2C BBQ10Keyboard */
// #define HW_CONTROLLER_I2C_BBQ10KB

#endif /* custom hardware */

#endif /* _HW_CONFIG_H_ */
