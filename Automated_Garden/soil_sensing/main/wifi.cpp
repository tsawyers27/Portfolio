#include "wifi.h"
#include "notification.h"
#include <string.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


// Network info
const char *ssid = "[Your WiFi network here]";
const char *pwd = "[Your WiFi password here]";

// Label for log messages
const char *TAG = "WIFI";

// Reconnection settings
const int max_retries {5};
int retry_count {0};

// Event group variables to prevent race conditions between WiFi and sensor
static EventGroupHandle_t wifi_event_group;
static constexpr EventBits_t WIFI_CONNECTED_BIT = BIT0;

void wifi_event_handler(void *handler_argument, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi started. Attempting to connect...");

        ESP_ERROR_CHECK(esp_wifi_connect());
    }
    
    // Connection established
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG, "Connected to the WiFi router.");
    }

    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconnect_info = (wifi_event_sta_disconnected_t *) event_data;

        ESP_LOGE(TAG, "WiFi connection failed. Reason code: %d", disconnect_info->reason);

        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);

        if (retry_count < max_retries)
        {
            retry_count++;

            ESP_LOGI(TAG, "Retrying WiFi connections: %d of %d", retry_count, max_retries);
            ESP_ERROR_CHECK(esp_wifi_connect());
        }
        else
        {
            ESP_LOGE(TAG, "Maximum WiFi retries reached.");
        }
    }

    // Router has assigned an IP address
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "ESP32 received an IP address.");
        ESP_LOGI(TAG, "Network connection established.");

        retry_count = 0;

        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_connection()
{
    // Initialize network interface and event handler loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Create a station network interface
    esp_netif_create_default_wifi_sta();

    // Create default WiFi driver config
    wifi_init_config_t wifi_driver_config = WIFI_INIT_CONFIG_DEFAULT();

    // Create WiFi event group
    wifi_event_group = xEventGroupCreate();

    if (wifi_event_group == NULL)
    {
        ESP_LOGE(TAG, "Failed to create WiFi event group.");
        return;
    }

    // Initialize WiFi driver
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_driver_config));

    // Register handler for all WiFi events
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL));

    // Register handler for "Received IP address" event
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL));

    // Create an empty WiFi connection config
    wifi_config_t wifi_config = {};

    // Copy network name into the station config
    memcpy(wifi_config.sta.ssid, ssid, strlen(ssid)+1);

    // Copy password into the station config
    memcpy(wifi_config.sta.password, pwd, strlen(pwd)+1);

    // Set ESP32 to station mode to connect to existing access points
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Give the WiFi driver the network config
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Start WiFi driver
    ESP_ERROR_CHECK(esp_wifi_start());
}

bool wifi_is_connected()
{
    EventBits_t bits = xEventGroupGetBits(wifi_event_group);

    return (bits & WIFI_CONNECTED_BIT) != 0;
}