/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "am2301_driver.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_random.h>

#include "dht.h"

static const char * TAG = "shtc3";

#define I2C_MASTER_SCL_IO CONFIG_SHTC3_I2C_SCL_PIN
#define I2C_MASTER_SDA_IO CONFIG_SHTC3_I2C_SDA_PIN
#define I2C_MASTER_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */

#define SHTC3_SENSOR_ADDR 0x70      /*!< I2C address of SHTC3 sensor */

typedef struct {
    am2301_sensor_config_t *config;
    esp_timer_handle_t timer;
    bool is_initialized = false;
} am2301_sensor_ctx_t;

static am2301_sensor_ctx_t s_ctx;

static void timer_cb_internal(void *arg)
{
    auto *ctx = (am2301_sensor_ctx_t *) arg;
    if (!(ctx && ctx->config)) {
        return;
    }

    float temp, humidity;
    esp_err_t err = dht_read_float_data(DHT_TYPE_AM2301, GPIO_NUM_0,
        &humidity, &temp);
    if (err != ESP_OK) {
        return;
    }
    if (ctx->config->temperature.cb) {
        ctx->config->temperature.cb(ctx->config->temperature.endpoint_id, temp, ctx->config->user_data);
    }
    if (ctx->config->humidity.cb) {
        ctx->config->humidity.cb(ctx->config->humidity.endpoint_id, humidity, ctx->config->user_data);
    }
}

esp_err_t am2301_sensor_init(am2301_sensor_config_t *config)
{
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    // we need at least one callback so that we can start notifying application layer
    if (config->temperature.cb == NULL || config->humidity.cb == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    if (s_ctx.is_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    // keep the pointer to config
    s_ctx.config = config;

    esp_timer_create_args_t args = {
        .callback = timer_cb_internal,
        .arg = &s_ctx,
    };

    esp_err_t err = esp_timer_create(&args, &s_ctx.timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_timer_create failed, err:%d", err);
        return err;
    }

    err = esp_timer_start_periodic(s_ctx.timer, config->interval_ms * 1000);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_timer_start_periodic failed: %d", err);
        return err;
    }

    s_ctx.is_initialized = true;
    ESP_LOGI(TAG, "shtc3 initialized successfully");

    return ESP_OK;
}