// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::overflowed()") {
  SECTION("returns false on a fresh object") {
    StaticJsonDocument<0> doc;
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    StaticJsonDocument<0> doc;
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1)> doc;
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1)> doc;
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) + 8> doc;
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    StaticJsonDocument<1> doc;
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1)> doc;
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) + 8> doc;
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    StaticJsonDocument<0> doc;
    doc.add(0);
    doc.clear();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains false after shrinkToFit()") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    doc.add(0);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains true after shrinkToFit()") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    doc.add(0);
    doc.add(0);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == true);
  }

  SECTION("return false after garbageCollect()") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    doc.add(0);
    doc.add(0);
    doc.garbageCollect();
    CHECK(doc.overflowed() == false);
  }
}
