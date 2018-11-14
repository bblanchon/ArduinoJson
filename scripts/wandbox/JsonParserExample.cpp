// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to deserialize a JSON document with ArduinoJson.

#include <iostream>
#include "ArduinoJson.h"

int main() {
  // Root JSON object
  //
  // Inside the brackets, 300 is the size of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<300> doc;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonObject which allocates in the heap.
  //
  // DynamicJsonObject doc(200);

  // JSON input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    std::cerr << "deserializeJson() failed: " << error.c_str() << std::endl;
    return 1;
  }

  // Get the root object in the document
  JsonObject root = doc.as<JsonObject>();

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = root["sensor"];
  long time = root["time"];
  double latitude = root["data"][0];
  double longitude = root["data"][1];

  // Print values.
  std::cout << sensor << std::endl;
  std::cout << time << std::endl;
  std::cout << latitude << std::endl;
  std::cout << longitude << std::endl;

  return 0;
}
