#include "relay.h"
#include "esp_log.h"

static const char *TAG = "relay";

#define RELAY_CHECK(a, str, action) if(!(a)) { \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str); \
        action; \
    }

esp_err_t relay_gpio_init(const gpio_num_t gpio_pin, void **ret_handle)
{
    gpio_reset_pin(gpio_pin);
	gpio_set_direction(gpio_pin, GPIO_MODE_INPUT_OUTPUT);

    gpio_num_t *handle = (gpio_num_t*)calloc(1, sizeof(gpio_num_t));
    *handle = gpio_pin;
    *ret_handle = (void *)handle;
    return ESP_OK;
}

esp_err_t relay_gpio_deinit(void *handle)
{
    if (handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    gpio_num_t *p_gpio = (gpio_num_t *)handle;
    esp_err_t ret = gpio_reset_pin(*p_gpio);

    if (ret != ESP_OK) {
        return ret;
    }
    free(p_gpio);

    return ESP_OK;
}


esp_err_t relay_gpio_set_on_off(void *handle, bool on_off)
{
    if (handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    gpio_num_t *p_gpio = (gpio_num_t *)handle;
    return gpio_set_level(*p_gpio, on_off);
}

relay_handle_t relay_create(const gpio_num_t gpio_pin)
{
    esp_err_t ret = ESP_OK;
    void *handle = NULL;
    ret = relay_gpio_init(gpio_pin, &handle);
    RELAY_CHECK(ESP_OK == ret, "Relay GPIO init failed", return NULL);
    return handle;
}