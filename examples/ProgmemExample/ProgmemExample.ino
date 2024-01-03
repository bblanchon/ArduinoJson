// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License
//
// This example shows the different ways you can use Flash strings with
// ArduinoJson.
//
// Use Flash strings sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char*, as they are more efficient in term of
// code size, speed, and memory usage.
//
// https://arduinojson.org/v7/example/progmem/

#include <ArduinoJson.h>

void setup() {
  JsonDocument doc;

  // You can use a Flash String as your JSON input.
  // WARNING: the strings in the input will be duplicated in the JsonDocument.
  deserializeJson(doc, F("{\"sensor\":\"gps\",\"time\":1351824120,"
                         "\"data\":[48.756080,2.302038]}"));

  // You can use a Flash String as a key to get a member from JsonDocument
  // No duplication is done.
  long time = doc[F("time")];

  // You can use a Flash String as a key to set a member of a JsonDocument
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonDocument.
  doc[F("time")] = time;

  // You can set a Flash String as the content of a JsonVariant
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonDocument.
  doc["sensor"] = F("gps");

  // It works with serialized() too:
  doc["sensor"] = serialized(F("\"gps\""));
  doc["sensor"] = serialized(F("\xA3gps"), 3);

  // You can compare the content of a JsonVariant to a Flash String
  if (doc["sensor"] == F("gps")) {
    // ...
  }
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any memory
// problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. It also tells why you
// should not abuse Flash strings with ArduinoJson.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
