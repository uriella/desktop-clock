#include "dht_and_arduino_json.h"

void updateTemperature(DHT dht, StaticJsonDocument<500> doc, std::ostream& output){
  
    float humid = dht.readHumidity();
    float celcius = dht.readTemperature();
    float fahren = dht.readTemperature(true);

    if (isnan(humid) || isnan(celcius) || isnan(fahren)){
        return;
    }

    float hif = dht.computeHeatIndex(fahren, humid);
    float hic = dht.computeHeatIndex(celcius, humid, false);
  
    doc["humid"] = humid;
    doc["celcius"] = celcius;
    doc["fahren"] = fahren;
    doc["heatIndexCelcius"] = hic;
    doc["heatIndexFahren"] = hif;

    serializeJson(doc, output);
    Serial.println();
}