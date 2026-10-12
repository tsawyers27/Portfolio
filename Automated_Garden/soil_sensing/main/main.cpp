#include "wifi.h"
#include "nvs_flash.h"
#include "soil_sensing.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Logging variable
static const char *TAG = "MAIN";

extern "C" void app_main(void)
{
    // Create non-volatile storage
    ESP_ERROR_CHECK(nvs_flash_init());

    // Attempt to connect to WiFi using provided configuration
    wifi_connection();

    // Read soil sensor data
    BaseType_t task_created = xTaskCreate(soil_sensor_task, "soil_sensor_task", 4096, NULL, 5, NULL);

    if (task_created == pdPASS)
    {
        ESP_LOGI(TAG, "Created soil sensor task");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to create soil sensor task");
    }
}