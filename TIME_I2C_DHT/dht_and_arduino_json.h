#ifndef DHT_AND_ARDUINO_JSON
#define DHT_AND_ARDUINO_JSON

#include <DHT.h>
#include <ArduinoJson.h>

void updateTemperature(DHT dht, StaticJsonDocument<500> doc, std::ostream& output);

#endif