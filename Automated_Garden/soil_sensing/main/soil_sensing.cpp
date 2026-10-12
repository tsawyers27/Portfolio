#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "notification.h"
#include "wifi.h"
#include "esp_random.h"
#include <algorithm>
#include <iterator>

// ADC unit and channel designations, adjust to fit your setup if needed
constexpr adc_unit_t SOIL_ADC_UNIT = ADC_UNIT_2;
constexpr adc_channel_t SOIL_ADC_CHANNEL = ADC_CHANNEL_3;

// Calibration variables
constexpr int DRY {2700};
constexpr int WET {40};

// Moisture thresholds (I chose these for tomatoes and cucumber, feel free to edit)
constexpr float DRY_THRESHOLD {60.0f};
constexpr float RESET_THRESHOLD {70.0f};

// Sampling and monitoring intervals
constexpr int NUM_SAMPLES {10};
constexpr TickType_t SAMPLE_DELAY {pdMS_TO_TICKS(100)};
constexpr TickType_t MONITOR_DELAY {pdMS_TO_TICKS(30000)};

// Message options to be sent when soil is dry and tracking variables
static const char *DRY_MESSAGES[] =
{
    "Larry the Cucumber: Hey, uh... not to be dramatic, but I could really use some water.",
    "Bob the Tomato: A responsible tomato knows when to ask for help. Please water the garden.",
    "Larry the Cucumber: Soooo... anybody know where we keep the water around here?",
    "Bob the Tomato: The soil is getting dry. This is officially a vegetable emergency.",
    "Larry the Cucumber: I've been thinking, and I think I'd be much happier with some water.",
    "Bob the Tomato: Larry, stop talking. We need water.",
    "Larry the Cucumber: If it's not too much trouble, the cucumbers would like to file a hydration request.",
    "Bob the Tomato: Garden moisture is low. Please water us before Larry starts singing.",
    "Larry the Cucumber: Is it just me, or is it getting a little crispy out here?",
    "Bob the Tomato: Larry says he's thirsty. For once, he's actually right.",
    "Larry the Cucumber: Hello from the garden! Everything is great except for the whole no-water situation.",
    "Bob the Tomato: Please water the plants before Larry turns this into a musical number."
};

constexpr size_t MESSAGE_COUNT = std::size(DRY_MESSAGES);

// Logging tag definition
static const char *TAG = "SOIL";

const char *get_random_dry_message()
{
    size_t message_index = esp_random() % MESSAGE_COUNT;

    return DRY_MESSAGES[message_index];
}

// Convert analog readout to 0-100% scale
float convert_to_percentage(int adc_raw)
{
    float moisture_percent = 100.0f * (DRY - adc_raw) / (DRY - WET);
    return std::clamp(moisture_percent,0.0f, 100.0f);
}

// Read sensor 10 times and take average to account for noise
int read_average_adc(adc_oneshot_unit_handle_t adc_handle)
{

    int total {0};
    int adc_raw {0};

    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, SOIL_ADC_CHANNEL, &adc_raw));

        total += adc_raw;

        vTaskDelay(SAMPLE_DELAY);
    }

    return total / NUM_SAMPLES;
}

// Initialize and monitor soil sensor
void soil_sensor_task(void *task_parameter)
{
    // Create ADC unit config
    adc_oneshot_unit_init_cfg_t init_config {};
    init_config.unit_id = SOIL_ADC_UNIT;

    // Create ADC handle
    adc_oneshot_unit_handle_t adc_handle;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    // Configure ADC channel
    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    channel_config.atten = ADC_ATTEN_DB_12;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, SOIL_ADC_CHANNEL, &channel_config));

    bool soil_is_dry {false};
    bool notification_pending {false};

    while (true)
    {
        // Get average soil reading
        int adc_average = read_average_adc(adc_handle);
        float moisture_percent = convert_to_percentage(adc_average);

        NotificationStatus notification_status = get_notification_status();

        if (notification_status == NotificationStatus::SUCCESS)
        {
            notification_pending = false;

            ESP_LOGI(TAG, "Notification delivered successfully.");
            reset_notification_status();
        }

        else if (notification_status == NotificationStatus::FAILED)
        {
            ESP_LOGW(TAG, "Notification failed. Will retry when possible.");
            reset_notification_status();
        }

        ESP_LOGI(TAG, "Average: %d | Moisture: %.1f%%", adc_average, moisture_percent);
        
        // Update soil state
        if (moisture_percent < DRY_THRESHOLD && !soil_is_dry)
        {
            soil_is_dry = true;
            notification_pending = true;

            ESP_LOGW(TAG, "I'm dying of thirst, please water me! Moisture: %.1f%%", moisture_percent);
        }
        else if (moisture_percent >= RESET_THRESHOLD && soil_is_dry)
        {
            soil_is_dry = false;
            notification_pending = true;

            ESP_LOGI(TAG, "Thanks for the water! Moisture: %.1f%%", moisture_percent);
        }

        // Send pending notification
        if (soil_is_dry && notification_pending)
        {
        if (wifi_is_connected() &&
            get_notification_status() == NotificationStatus::IDLE)
        {
            mark_notification_in_progress();

            const char *message = get_random_dry_message();

            BaseType_t task_created = xTaskCreate(ntfy_task,"ntfy_task",8192,const_cast<char *>(message),5,NULL);

            if (task_created == pdPASS)
            {
                ESP_LOGI(TAG, "Notification task created.");
            }
            else
            {
                reset_notification_status();
                ESP_LOGE(TAG, "Failed to create notification task.");
            }
        }
        else if (!wifi_is_connected())
        {
            ESP_LOGW(TAG, "Notification pending until WiFi reconnects.");
        }
    }
    
    // Notify user that appropriate moisture level has been reached
    else if (!soil_is_dry && notification_pending)
    {
        if (wifi_is_connected() && get_notification_status() == NotificationStatus::IDLE)
        {
            mark_notification_in_progress();

            const char *wet_message = "Thanks for the water!";

            BaseType_t task_created = xTaskCreate(ntfy_task, "ntfy_task",8192, const_cast<char *>(wet_message), 5, NULL);

            if (task_created == pdPASS)
            {
                ESP_LOGI(TAG, "Notification task created.");
            }
            else
            {
                reset_notification_status();
                ESP_LOGE(TAG, "Failed to create notification task.");
            }
        }
        else if (!wifi_is_connected())
        {
            ESP_LOGW(TAG, "Notification pending until WiFi reconnects.");
        }
    }

        vTaskDelay(MONITOR_DELAY);
    }
}