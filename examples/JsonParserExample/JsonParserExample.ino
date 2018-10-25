// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows how to deserialize a JSON document with ArduinoJson.

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the size of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument doc(200);

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
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Get the root object in the document
  JsonObject root = doc.as<JsonObject>();

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  const char* sensor = root["sensor"];
  long time = root["time"];
  double latitude = root["data"][0];
  double longitude = root["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}

void loop() {
  // not used in this example
}

// Visit https://arduinojson.org/v6/example/parser/ for more.
