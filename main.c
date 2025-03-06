#include <string.h>  
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "wifi_provisioning/manager.h"
#include "wifi_provisioning/scheme_softap.h"

#define WIFI_SSID "TestAP"
#define WIFI_PASSWORD "Password"  

static const char *TAG = "wifi softAP";

void wifi_init_softap(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set(TAG, ESP_LOG_INFO);

    ESP_ERROR_CHECK( esp_netif_init() );
    ESP_ERROR_CHECK( esp_event_loop_create_default() );

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PASSWORD,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    if (strlen(WIFI_PASSWORD) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );

    ESP_LOGI(TAG, "Access Point '%s' started with password '%s'", WIFI_SSID, WIFI_PASSWORD);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_init_softap();
}
