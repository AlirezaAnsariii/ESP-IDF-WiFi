#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

static const char *TAG = "wifi_example";

esp_err_t wifi_event_handler(void *ctx, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "Station started");
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Connected to WiFi");
            break;
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "Got IP");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from WiFi, retrying...");
            esp_wifi_connect();
            break;
        default:
            break;
    }
    return ESP_OK;
}

void wifi_init_sta() {
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_ERROR_CHECK(nvs_flash_init());

    esp_netif_init();
    esp_event_loop_create_default();
    
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "SSID",  
            .password = "PASSWD@",  
            .bssid_set = false,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing WiFi...");
    wifi_init_sta();
    
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
}
