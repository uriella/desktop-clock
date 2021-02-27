#ifndef DHT
#define DHT

#include <string.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

// DHT timer precision in microseconds
#define DHT_TIMER_INTERVAL 2
#define DHT_DATA_BITS 40
#define DHT_DATA_BYTES (DHT_DATA_BITS / 8)

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
#define PORT_ENTER_CRITICAL portENTER_CRITICAL(&mux)
#define PORT_EXIT_CRITICAL portEXIT_CRITICAL(&mux)

#define CHECK_ARG(VAL)                      \
  do {                                      \
    if (!(VAL)) return ESP_ERR_INVALID_ARG; \
  } while (0)

#define CHECK_LOGE(x, msg, ...)              \
  do {                                       \
    esp_err_t __;                            \
    if ((__ = x) != ESP_OK) {                \
      PORT_EXIT_CRITICAL;                    \
      ESP_LOGE(DHT_TAG, msg, ##__VA_ARGS__); \
      return __;                             \
    }                                        \
  } while (0)

typedef enum {
  DHT_TYPE_DHT11 = 0,  //!< DHT11
  DHT_TYPE_AM2301,     //!< AM2301 (DHT21, DHT22, AM2302, AM2321)
  DHT_TYPE_SI7021      //!< Itead Si7021
} dht_sensor_type_t;

static const char *DHT_TAG = "dht";

static esp_err_t dht_await_pin_state(gpio_num_t pin, uint32_t timeout,
                                     int expected_pin_state,
                                     uint32_t *duration);

static inline esp_err_t dht_fetch_data(dht_sensor_type_t sensor_type,
                                       gpio_num_t pin,
                                       uint8_t data[DHT_DATA_BYTES]);

static inline int16_t dht_convert_data(dht_sensor_type_t sensor_type,
                                       uint8_t msb, uint8_t lsb);

esp_err_t dht_read_data(dht_sensor_type_t sensor_type, gpio_num_t pin,
                        int16_t *humidity, int16_t *temperature);

esp_err_t dht_read_float_data(dht_sensor_type_t sensor_type, gpio_num_t pin,
                              float *humidity, float *temperature);

#endif