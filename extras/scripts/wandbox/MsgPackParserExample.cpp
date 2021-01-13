// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.

#include <iostream>
#include "ArduinoJson.h"

int main() {
  // Allocate the JSON document
  //
  // Inside the brackets, 300 is the size of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<300> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonObject which allocates in the heap.
  //
  // DynamicJsonObject doc(200);

  // MessagePack input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
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

  // doc of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  DeserializationError error = deserializeMsgPack(doc, input);

  // Test if parsing succeeds.
  if (error) {
    std::cerr << "deserializeMsgPack() failed: " << error.c_str() << std::endl;
    return 1;
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
  std::cout << sensor << std::endl;
  std::cout << time << std::endl;
  std::cout << latitude << std::endl;
  std::cout << longitude << std::endl;

  return 0;
}
