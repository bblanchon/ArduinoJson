// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include <iostream>
#include "ArduinoJson.h"

int main() {
  // The JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Make our document be an object
  JsonObject root = doc.to<JsonObject>();

  // Add values in the object
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root.set<long>("time", 1351824120);
  root["sensor"] = "gps";
  root["time"] = 1351824120;

  // Add an array.
  //
  JsonArray data = root.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(root, std::cout);
  // This prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  std::cout << std::endl;

  serializeJsonPretty(root, std::cout);
  // This prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }

  return 0;
}
