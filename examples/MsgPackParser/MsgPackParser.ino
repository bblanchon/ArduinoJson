// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License
//
// This example shows how to deserialize a MessagePack document with
// ArduinoJson.
//
// https://arduinojson.org/v6/example/msgpack-parser/

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;

  // Allocate the JSON document
  JsonDocument doc;

  // MessagePack input string.
  uint8_t input[] = {131, 166, 115, 101, 110, 115, 111, 114, 163, 103, 112, 115,
                     164, 116, 105, 109, 101, 206, 80,  147, 50,  248, 164, 100,
                     97,  116, 97,  146, 203, 64,  72,  96,  199, 58,  188, 148,
                     112, 203, 64,  2,   106, 146, 230, 33,  49,  169};
  // This MessagePack document contains:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [48.75608, 2.302038]
  // }

  DeserializationError error = deserializeMsgPack(doc, input);

  // Test if parsing succeeded.
  if (error) {
    Serial.print("deserializeMsgPack() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}

void loop() {
  // not used in this example
}
