#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <esp_matter.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include "bsp/esp-bsp.h"

#include "reset_button_driver.h"

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

reset_button_driver_handle_t app_driver_reset_button_init()
{
    /* Onboard button to be used for factory reset in app_main */

    button_config_t btn_cfg;
    btn_cfg.type = BUTTON_TYPE_GPIO;
    btn_cfg.gpio_button_config.gpio_num = 9;
    btn_cfg.gpio_button_config.active_level = 0;

    button_handle_t btn = iot_button_create(&btn_cfg);
    
    return (reset_button_driver_handle_t)btn;
}
