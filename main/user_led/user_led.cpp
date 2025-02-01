#include "user_led.h"

static const gpio_num_t USER_LED_GPIO = GPIO_NUM_8;

void user_led_gpio_init()
{
    gpio_reset_pin(USER_LED_GPIO);
	gpio_set_direction(USER_LED_GPIO, GPIO_MODE_INPUT_OUTPUT);
    user_led_set_on_off(false);
}

void user_led_gpio_deinit()
{
    gpio_reset_pin(USER_LED_GPIO);
}

void user_led_set_on_off(bool on_off)
{
    gpio_set_level(USER_LED_GPIO, !on_off);
}

void user_led_flash_n(TickType_t duration, size_t n_times)
{
    for(size_t counter = 0; counter < n_times; counter++)
    {
        user_led_set_on_off(true);
        vTaskDelay(duration/portTICK_PERIOD_MS);
        user_led_set_on_off(false);
        vTaskDelay(duration/portTICK_PERIOD_MS);
    }
}