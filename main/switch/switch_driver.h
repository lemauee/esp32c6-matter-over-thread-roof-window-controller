#pragma once

#include <esp_err.h>
#include <esp_matter.h>

/** Default attribute values used during initialization */
#define DEFAULT_POWER true

constexpr size_t N_SWITCHES = 4;

typedef void *switch_driver_handle_t;

/** Initialize the switch driver
 *
 * This initializes the switch driver associated with the selected board.
 *
 * @param[in] i_gpio Index of the switch input to use (0 to 3).
 * 
 * @return Handle on success.
 * @return NULL in case of failure.
 */
switch_driver_handle_t app_driver_switch_init(const size_t i_gpio);