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

    button_handle_t btns[BSP_BUTTON_NUM];
    ESP_ERROR_CHECK(bsp_iot_button_create(btns, NULL, BSP_BUTTON_NUM));
    
    return (reset_button_driver_handle_t)btns[0];
}
