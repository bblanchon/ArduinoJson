// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include <iostream>
#include "ArduinoJson.h"

int main() {
  // Allocate the JSON document
  JsonDocument doc;

  // Add values in the document.
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array
  JsonArray data = doc["data"].to<JsonArray>();
  data.add(48.756080);
  data.add(2.302038);

  // Generate the minified JSON and send it to STDOUT
  serializeJson(doc, std::cout);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // Start a new line
  std::cout << std::endl;

  // Generate the prettified JSON and send it to STDOUT
  serializeJsonPretty(doc, std::cout);
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
}
