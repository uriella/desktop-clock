#include "wifi_init.h"

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;
      case WIFI_EVENT_STA_DISCONNECTED: {
        if (s_retry_num < DEFAULT_ESP_MAXIMUM_RETRY) {
          esp_wifi_connect();
          s_retry_num++;
          ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
        } else {
          xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(WIFI_TAG, "connect to the AP fail");
      } break;
    }
  } else if (event_base == IP_EVENT) {
    switch (event_id) {
      case IP_EVENT_STA_GOT_IP: {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        const tcpip_adapter_ip_info_t* ip_info = &event->ip_info;

        ESP_LOGI(WIFI_TAG, "WiFi Got IP Address");
        ESP_LOGI(WIFI_TAG, "WIFI IP:" IPSTR, IP2STR(&ip_info->ip));
        ESP_LOGI(WIFI_TAG, "WIFI MASK:" IPSTR, IP2STR(&ip_info->netmask));
        ESP_LOGI(WIFI_TAG, "WIFI GW:" IPSTR, IP2STR(&ip_info->gw));

        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
      } break;
    }
  }
}

void wifi_init_sta(void) {
  // TCP/IP event handling & group (akin to flags and semaphores)
  s_wifi_event_group = xEventGroupCreate();

  ESP_ERROR_CHECK(esp_netif_init());

  esp_netif_ip_info_t ipInfo;

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  //
  esp_netif_t* netif = esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

  char* ip = "192.168.X.X";
  char* gateway = "192.168.X.X";
  char* netmask = "255.255.255.X";
  int dhcp_mode = 0;
  if (!dhcp_mode) {
    esp_netif_dhcpc_stop(netif);
    ip4addr_aton((const char*)ip, &ipInfo.ip);
    ip4addr_aton((const char*)gateway, &ipInfo.gw);
    ip4addr_aton((const char*)netmask, &ipInfo.netmask);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(netif, &ipInfo));
  }

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = DEFAULT_ESP_WIFI_SSID,
              .password = DEFAULT_ESP_WIFI_PASS,
              .threshold.authmode = WIFI_AUTH_WPA2_PSK,

              .pmf_cfg = {.capable = true, .required = false},
          },
  };

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ipInfo));

  ESP_LOGI(WIFI_TAG, "wifi_init_sta finished.");

  /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or
   * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT). The
   * bits are set by event_handler() (see above) */
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE, pdFALSE, portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we
   * can test which event actually happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(WIFI_TAG, "connected to ap SSID:%s password:%s",
             DEFAULT_ESP_WIFI_SSID, DEFAULT_ESP_WIFI_PASS);
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(WIFI_TAG, "Failed to connect to SSID:%s, password:%s",
             DEFAULT_ESP_WIFI_SSID, DEFAULT_ESP_WIFI_PASS);
  } else {
    ESP_LOGE(WIFI_TAG, "UNEXPECTED EVENT");
  }

  /* The event will not be processed after unregister */
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
  vEventGroupDelete(s_wifi_event_group);
}