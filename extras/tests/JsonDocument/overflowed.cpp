// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("JsonDocument::overflowed()") {
  TimebombAllocator allocator(10);
  JsonDocument doc(&allocator);

  SECTION("returns false on a fresh object") {
    allocator.setCountdown(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    allocator.setCountdown(0);
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    allocator.setCountdown(2);
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    allocator.setCountdown(0);
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    allocator.setCountdown(3);
    doc.add(std::string("example"));
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    allocator.setCountdown(0);
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    allocator.setCountdown(0);
    deserializeJson(doc, "[1, 2]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    allocator.setCountdown(3);
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    allocator.setCountdown(0);
    doc.add(0);
    doc.clear();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains false after shrinkToFit()") {
    allocator.setCountdown(2);
    doc.add(0);
    allocator.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains true after shrinkToFit()") {
    allocator.setCountdown(0);
    doc.add(0);
    allocator.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == true);
  }
}
