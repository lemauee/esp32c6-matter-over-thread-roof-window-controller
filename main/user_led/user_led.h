#pragma once

#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

/**
 * @brief Initialize the specific user led GPIO.
 */
void user_led_gpio_init();

/**
 * @brief Deinitialize the specific user led GPIO.
 */
void user_led_gpio_deinit();

/**
 * @brief Set the specific GPIO's level to turn the led on or off.
 * 
 * @param on_off True to turn the led on, false to turn it off.
 */
void user_led_set_on_off(bool on_off);

/**
 * @brief Flash the led. 
 * 
 * @param duration Timespan of the on/off period in ms.
 * @param n_times Number of repetitions.
 */
void user_led_flash_n(TickType_t duration, size_t n_times);