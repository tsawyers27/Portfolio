idf_component_register(
    SRCS "soil_sensing.cpp" "wifi.cpp" "notification.cpp" "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES
        esp_adc
        esp_event
        esp_http_client
        esp_netif
        esp-tls
        esp_wifi
        mbedtls
        nvs_flash
)