// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait serial port initialization
  }

  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = "gps";
  root["time"] = 1351824120;

  JsonArray& data = root.createNestedArray("data");
  data.add(double_with_n_digits(48.756080, 6));
  data.add(double_with_n_digits(2.302038, 6));

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
