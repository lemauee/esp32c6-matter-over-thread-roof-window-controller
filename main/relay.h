#pragma once

#include "driver/gpio.h"

typedef void *relay_handle_t; /*!< Relay operation handle */

/**
 * @brief Initialize the specific GPIO to work as a relay controller
 *
 * @param gpio_pin GPIO pin.
 * @param ret_handle GPIO_handle.
 * @return esp_err_t
 *     - ESP_OK success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t relay_gpio_init(gpio_num_t gpio_pin, void **ret_handle);

/**
 * @brief Deinitialize the specific GPIO that works as a relay
 *
 * @param handle GPIO handle
 * @return esp_err_t
 *     - ESP_OK success
 */
esp_err_t relay_gpio_deinit(void *handle);

/**
 * @brief Set the specific GPIO's level to make the relay ON or OFF
 *
 * @param handle GPIO handle
 * @param on_off True to turn the relay on, false to turn it off.
 * @return esp_err_t
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG GPIO number error
 */
esp_err_t relay_gpio_set_on_off(void *handle, bool on_off);

/**
 * @brief create a relay instance with GPIO number
 *
 * @param gpio_pin GPIO pin of the relay.
 * @return relay_handle_t handle of the relay, NULL if create failed.
 */
relay_handle_t relay_create(const gpio_num_t gpio_pin);