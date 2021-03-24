#ifndef WIFI_INIT
#define WIFI_INT

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"

#define DEFAULT_ESP_WIFI_SSID "yourssid"
#define DEFAULT_ESP_WIFI_PASS "password"
#define DEFAULT_ESP_MAXIMUM_RETRY 5

class WiFi {
 public:
  static constexpr char* tag = "wifi station";
  int s_retry_num = 0;

  static EventGroupHandle_t s_wifi_event_group;

  static void event_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data);

  static void incRetry();

  void init();
};
/* FreeRTOS event group to signal when we are connected*/

/* The event group allows multiple bits for each event, but we only care about
 * two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#endif