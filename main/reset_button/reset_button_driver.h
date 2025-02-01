#pragma once

#include <esp_err.h>
#include <esp_matter.h>

typedef void *reset_button_driver_handle_t;

/** Initialize the reset button driver
 *
 * This initializes the reset button driver associated with the selected board.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
reset_button_driver_handle_t app_driver_reset_button_init();