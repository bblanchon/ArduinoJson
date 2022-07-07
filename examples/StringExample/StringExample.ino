// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.
//
// https://arduinojson.org/v6/example/string/

#include <ArduinoJson.h>

void setup() {
  DynamicJsonDocument doc(1024);

  // You can use a String as your JSON input.
  // WARNING: the string in the input  will be duplicated in the JsonDocument.
  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  deserializeJson(doc, input);

  // You can use a String as a key to get a member from JsonDocument
  // No duplication is done.
  long time = doc[String("time")];

  // You can use a String as a key to set a member of a JsonDocument
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  doc[String("time")] = time;

  // You can get the content of a JsonVariant as a String
  // No duplication is done, at least not in the JsonDocument.
  String sensor = doc["sensor"];

  // Unfortunately, the following doesn't work (issue #118):
  // sensor = doc["sensor"]; // <-  error "ambiguous overload for 'operator='"
  // As a workaround, you need to replace by:
  sensor = doc["sensor"].as<String>();

  // You can set a String as the content of a JsonVariant
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  doc["sensor"] = sensor;

  // It works with serialized() too:
  doc["sensor"] = serialized(sensor);

  // You can also concatenate strings
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  doc[String("sen") + "sor"] = String("gp") + "s";

  // You can compare the content of a JsonObject with a String
  if (doc["sensor"] == sensor) {
    // ...
  }

  // Lastly, you can print the resulting JSON to a String
  // WARNING: it doesn't replace the content but appends to it
  String output;
  serializeJson(doc, output);
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. On several occasions, it
// shows how you can avoid String in your program.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
