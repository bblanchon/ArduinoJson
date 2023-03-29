// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonDocument::overflowed()") {
  SECTION("returns false on a fresh object") {
    JsonDocument doc(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    JsonDocument doc(0);
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    JsonDocument doc(sizeofArray(1));
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    JsonDocument doc(sizeofArray(1));
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    JsonDocument doc(sizeofArray(1) + 8);
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    JsonDocument doc(1);
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    JsonDocument doc(sizeofArray(1));
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    JsonDocument doc(sizeofArray(1) + 8);
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    JsonDocument doc(0);
    doc.add(0);
    doc.clear();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains false after shrinkToFit()") {
    JsonDocument doc(sizeofArray(1));
    doc.add(0);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains true after shrinkToFit()") {
    JsonDocument doc(sizeofArray(1));
    doc.add(0);
    doc.add(0);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == true);
  }

  SECTION("return false after garbageCollect()") {
    JsonDocument doc(sizeofArray(1));
    doc.add(0);
    doc.add(0);
    doc.garbageCollect();
    CHECK(doc.overflowed() == false);
  }
}
