/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <esp_matter.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include "bsp/esp-bsp.h"

#include <app_priv.h>

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

static const char *TAG = "app_driver";
extern uint16_t relay_0_endpoint_id;
extern uint16_t relay_1_endpoint_id;

extern uint16_t switch_0_endpoint_id;

constexpr size_t N_RELAYS = 4;
constexpr gpio_num_t RELAY_GPIOS[N_RELAYS] = {GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_22, GPIO_NUM_23};
constexpr size_t N_GPIOS = 4;
constexpr int32_t SWITCH_GPIOS[N_GPIOS] = {1, 2, 3, 15};

static esp_err_t app_driver_relay_set_power(relay_handle_t handle, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    relay_gpio_set_on_off(handle,val->val.b);
    ESP_LOGI(TAG, "Relay set power: %d", val->val.b);
    return err;
}

static void app_driver_button_down_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "Switch engaged");
    uint16_t endpoint_id = switch_0_endpoint_id;

    uint8_t newPosition = 1;

    chip::DeviceLayer::SystemLayer().ScheduleLambda([endpoint_id, newPosition]() {
        chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(endpoint_id, newPosition);
        // SwitchLatched event takes newPosition as event data
        switch_cluster::event::send_switch_latched(endpoint_id, newPosition);
    });
}

static void app_driver_button_up_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "Switch disengaged");
    uint16_t endpoint_id = switch_0_endpoint_id;

    uint8_t newPosition = 0;

    chip::DeviceLayer::SystemLayer().ScheduleLambda([endpoint_id, newPosition]() {
        chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(endpoint_id, newPosition);
        // SwitchLatched event takes newPosition as event data
        switch_cluster::event::send_switch_latched(endpoint_id, newPosition);
    });
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    if (endpoint_id == relay_0_endpoint_id) {
        // TODO: This if-tree (no switch due to cluster ids not being constexpr) could probably just be a map
        if (cluster_id == OnOff::Id) {
            relay_handle_t relay = (relay_handle_t)driver_handle;
            ESP_LOGI(TAG, "Relay 0");
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                err = app_driver_relay_set_power(relay, val);
            }
        }
    }
    else if (endpoint_id == relay_1_endpoint_id) {
        // TODO: This if-tree (no switch due to cluster ids not being constexpr) could probably just be a map
        if (cluster_id == OnOff::Id) {
            relay_handle_t relay = (relay_handle_t)driver_handle;
            ESP_LOGI(TAG, "Relay 1");
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                err = app_driver_relay_set_power(relay, val);
            }
        }
    }
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

app_driver_handle_t app_driver_relay_init(const size_t i_gpio)
{   
    /* Initialize relay */
    return (app_driver_handle_t)relay_create(RELAY_GPIOS[i_gpio]);
}

app_driver_handle_t app_driver_switch_init(const size_t i_gpio)
{
    /* Latching 230V switches connected via optocouplers to EVB */

    button_config_t button_config{};
    button_config.type = BUTTON_TYPE_GPIO;
    button_config.gpio_button_config.gpio_num = SWITCH_GPIOS[i_gpio];
    button_config.gpio_button_config.active_level = 0;
    button_handle_t button_handle{iot_button_create(&button_config)};
    if (button_handle == NULL){
        return NULL;
    }
    ESP_ERROR_CHECK(iot_button_register_cb(button_handle, BUTTON_PRESS_DOWN, app_driver_button_down_cb, NULL));
    ESP_ERROR_CHECK(iot_button_register_cb(button_handle, BUTTON_PRESS_UP, app_driver_button_up_cb, NULL));

    return (app_driver_handle_t)button_handle;
}

app_driver_handle_t app_driver_reset_button_init()
{
    /* Onboard button to be used for factory reset in app_main */

    button_handle_t btns[BSP_BUTTON_NUM];
    ESP_ERROR_CHECK(bsp_iot_button_create(btns, NULL, BSP_BUTTON_NUM));
    
    return (app_driver_handle_t)btns[0];
}
