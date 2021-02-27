#include "dht.h"

void updateTemperature(DHT dht) {
  float humid = dht.readHumidity();
  float celcius = dht.readTemperature();
  float fahren = dht.readTemperature(true);

  if (isnan(humid) || isnan(celcius) || isnan(fahren)) {
    return;
  }

  float hif = dht.computeHeatIndex(fahren, humid);
  float hic = dht.computeHeatIndex(celcius, humid, false);
}