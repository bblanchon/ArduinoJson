// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);

  StaticJsonBuffer<200> jsonBuffer;

  JsonArray& array = jsonBuffer.createArray();
  array.add(48.756080, 6);  // 6 is the number of decimals to print
  array.add(2.302038, 6);   // if not specified, 2 digits are printed

  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = "gps";
  root["time"] = 1351824120;
  root["data"] = array;

  root.printTo(Serial);
  // This prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  Serial.println();

  root.prettyPrintTo(Serial);
  // This prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
}

void loop() {
  // not used in this example
}