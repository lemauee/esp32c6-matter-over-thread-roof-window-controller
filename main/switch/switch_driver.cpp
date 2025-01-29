#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <esp_matter.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include "bsp/esp-bsp.h"

#include "switch_driver.h"

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

static const char *TAG = "switch_driver";

extern uint16_t switch_endpoint_ids[N_SWITCHES];

constexpr int32_t SWITCH_GPIOS[N_SWITCHES] = {1, 2, 3, 15};

static void app_driver_button_down_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "Switch engaged");
    uint16_t endpoint_id = *(int32_t*)data;

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
    uint16_t endpoint_id = *(int32_t*)data;

    uint8_t newPosition = 0;

    chip::DeviceLayer::SystemLayer().ScheduleLambda([endpoint_id, newPosition]() {
        chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(endpoint_id, newPosition);
        // SwitchLatched event takes newPosition as event data
        switch_cluster::event::send_switch_latched(endpoint_id, newPosition);
    });
}

switch_driver_handle_t app_driver_switch_init(const size_t i_gpio)
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
    ESP_ERROR_CHECK(iot_button_register_cb(button_handle, BUTTON_PRESS_DOWN, app_driver_button_down_cb, &switch_endpoint_ids[i_gpio]));
    ESP_ERROR_CHECK(iot_button_register_cb(button_handle, BUTTON_PRESS_UP, app_driver_button_up_cb, &switch_endpoint_ids[i_gpio]));

    return (switch_driver_handle_t)button_handle;
}