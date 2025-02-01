#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <esp_matter.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include "bsp/esp-bsp.h"

#include "relay_driver.h"

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

static const char *TAG = "relay_driver";

extern uint16_t relay_endpoint_ids[N_RELAYS];

constexpr gpio_num_t RELAY_GPIOS[N_RELAYS] = {GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_22, GPIO_NUM_23};

static esp_err_t app_driver_relay_set_power(relay_handle_t handle, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    relay_gpio_set_on_off(handle,val->val.b);
    ESP_LOGI(TAG, "Relay set power: %d", val->val.b);
    return err;
}

esp_err_t app_driver_relay_set_defaults(uint16_t endpoint_id)
{
    esp_err_t err = ESP_OK;
    void *priv_data = endpoint::get_priv_data(endpoint_id);
    relay_handle_t relay = (relay_handle_t)priv_data;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    /* Setting power */
    attribute_t *attribute  = attribute::get(endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);
    err |= app_driver_relay_set_power(relay, &val);

    return err;
}

relay_driver_handle_t app_driver_relay_init(const size_t i_gpio)
{   
    /* Initialize relay */
    return (relay_driver_handle_t)relay_create(RELAY_GPIOS[i_gpio]);
}

esp_err_t app_driver_attribute_update(relay_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    for (size_t i{0}; i < N_RELAYS; i++){
        if (endpoint_id == relay_endpoint_ids[i]) {
            if (cluster_id == OnOff::Id) {
                relay_handle_t relay = (relay_handle_t)driver_handle;
                ESP_LOGI(TAG, "Relay %zu", i);
                if (attribute_id == OnOff::Attributes::OnOff::Id) {
                    err = app_driver_relay_set_power(relay, val);
                }
            }
        }
    }
    return err;
}
