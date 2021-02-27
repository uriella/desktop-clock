#include <stdio.h>
#include <string.h>

#include "dht11.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "wifi_init.h"

void vTaskTemperature(void *pvParameters) {
  const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
  DHT11_init(GPIO_NUM_27);

  for (;;) {
    if (DHT11_read().temperature)
      printf("Temperature is %d \n", DHT11_read().temperature);
    if (DHT11_read().humidity)
      printf("Humidity is %d\n", DHT11_read().humidity);
    if (DHT11_read().status) printf("Status code is %d\n", DHT11_read().status);
    vTaskDelay(xDelay);
  }
}

void app_main(void) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(WIFI_TAG, "ESP_WIFI_MODE_STA");

  wifi_init_sta();
  xTaskCreate(vTaskTemperature, "temperature", configMINIMAL_STACK_SIZE * 3,
              NULL, 5, NULL);
}