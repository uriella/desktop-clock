#include <iostream>
#include <string>

#include "../include/wifi.hpp"
#include "dht11.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

using namespace std;
extern "C" {
void app_main();
}

void vTaskTemperature(void *pvParameters) {
  const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
  DHT11_init(GPIO_NUM_27);

  for (;;) {
    if (DHT11_read().temperature)
      cout << "Temperature is" << DHT11_read().temperature << endl;
    if (DHT11_read().humidity)
      cout << "Humidity is" << DHT11_read().humidity << endl;
    if (DHT11_read().status)
      cout << "Status code is" << DHT11_read().status << endl;
    vTaskDelay(xDelay);
  }
}

void app_main(void) {
  // Initialize NVS
  WiFi wifi;
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(WiFi::tag, "ESP_WIFI_MODE_STA");

  wifi.init();
  // xTaskCreate(vTaskTemperature, "temperature", configMINIMAL_STACK_SIZE * 3,
  // NULL, 5, NULL);
}