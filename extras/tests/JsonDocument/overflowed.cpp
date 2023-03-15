// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::overflowed()") {
  SECTION("returns false on a fresh object") {
    DynamicJsonDocument doc(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    DynamicJsonDocument doc(0);
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1) + 8);
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    DynamicJsonDocument doc(1);
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1));
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    DynamicJsonDocument doc(JSON_ARRAY_SIZE(1) + 8);
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    DynamicJsonDocument doc(0);
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
