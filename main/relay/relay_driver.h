#pragma once

#include <esp_err.h>
#include <esp_matter.h>
#include <relay.h>

constexpr size_t N_RELAYS = 4;

/** Default attribute values used during initialization */
#define DEFAULT_POWER true

typedef void *relay_driver_handle_t;

/** Initialize the relay driver
 *
 * This initializes the relay driver associated with the selected board.
 *
 * @param[in] i_gpio Index of the relay to use (0 to 3).
 * 
 * @return Handle on success.
 * @return NULL in case of failure.
 */
relay_driver_handle_t app_driver_relay_init(const size_t i_gpio);

/** Set defaults for relay driver
 *
 * Set the attribute drivers to their default values from the created data model.
 *
 * @param[in] endpoint_id Endpoint ID of the driver.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_relay_set_defaults(uint16_t endpoint_id);

/** Driver Update
 *
 * This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 *
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_attribute_update(relay_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val);