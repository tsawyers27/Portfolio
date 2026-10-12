#include "notification.h"
#include <string.h>
#include "esp_crt_bundle.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <atomic>

// Store notification status
static std::atomic<NotificationStatus> notification_status {NotificationStatus::IDLE};

// Label for log messages
const char *HTTP_TAG = "HTTP";

bool send_ntfy_message(const char *message)
{
    // ntfy URL and message info
    const char *ntfy_url = "https://ntfy.sh/[Your ntfy topic here]";

    // Create HTTP config struct
    esp_http_client_config_t http_config = {};
    http_config.url = ntfy_url;
    http_config.crt_bundle_attach = esp_crt_bundle_attach;

    // Create HTTP client using the config
    esp_http_client_handle_t client = esp_http_client_init(&http_config);

    // Stop if the HTTP client could not be created
    if (client == NULL)
    {
        ESP_LOGE(HTTP_TAG, "Failed to create HTTP client.");
        return false;
    }

    // Tell the HTTP client to use a POST request
    ESP_ERROR_CHECK(esp_http_client_set_method(client, HTTP_METHOD_POST));

    // Tell the server that the request body contains plain text
    ESP_ERROR_CHECK(esp_http_client_set_header(client, "Content-Type", "text/plain"));

    // Attach the message to the request
    //ESP_ERROR_CHECK(esp_http_client_set_post_field(client, message, strlen(message)));

    esp_err_t post_field_result = esp_http_client_set_post_field(client, message, strlen(message));
    if (post_field_result != ESP_OK)
    {
        ESP_LOGE(HTTP_TAG, "Failed to set POST message: %s", esp_err_to_name(post_field_result));
        
        // Cleanup HTTP client
        esp_err_t cleanup_result = esp_http_client_cleanup(client);

        if (cleanup_result != ESP_OK)
        {
            ESP_LOGE(HTTP_TAG, "HTTP client cleanup failed: %s", esp_err_to_name(cleanup_result));
        }
        
        return false;
    }

    // Send the POST request
    esp_err_t request_result = esp_http_client_perform(client);

    bool notification_sent = false;

    if (request_result == ESP_OK)
    {
        int status_code = esp_http_client_get_status_code(client);

        ESP_LOGI(HTTP_TAG, "HTTP POST completed. Status code: %d", status_code);

        if (status_code >= 200 && status_code < 300)
        {
            notification_sent = true;
        }
    }
    else
    {
        ESP_LOGE(HTTP_TAG, "HTTP POST failed: %s", esp_err_to_name(request_result));
    }

    ESP_ERROR_CHECK(esp_http_client_cleanup(client));

    return notification_sent;
}

void ntfy_task(void *task_parameter)
{
    const char *message = static_cast<const char *>(task_parameter);

    notification_status = NotificationStatus::IN_PROGRESS;

    // Send ntfy message and report successful POST
    bool success = send_ntfy_message(message);

    if (success)
    {
        notification_status = NotificationStatus::SUCCESS;
    }
    else
    {
        notification_status = NotificationStatus::FAILED;
    }

    // Delete this task after the message has been sent
    vTaskDelete(NULL);
}

NotificationStatus get_notification_status()
{
    return notification_status.load();
}

void reset_notification_status()
{
    notification_status = NotificationStatus::IDLE;
}

void mark_notification_in_progress()
{
    notification_status = NotificationStatus::IN_PROGRESS;
}