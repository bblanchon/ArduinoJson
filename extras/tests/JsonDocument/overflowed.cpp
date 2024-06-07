// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::overflowed()") {
  TimebombAllocator timebomb(10);
  JsonDocument doc(&timebomb);

  SECTION("returns false on a fresh object") {
    timebomb.setCountdown(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    timebomb.setCountdown(0);
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    timebomb.setCountdown(2);
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    timebomb.setCountdown(0);
    doc.add("example"_s);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    timebomb.setCountdown(3);
    doc.add("example"_s);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    timebomb.setCountdown(0);
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    timebomb.setCountdown(0);
    deserializeJson(doc, "[1, 2]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    timebomb.setCountdown(3);
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    timebomb.setCountdown(0);
    doc.add(0);
    doc.clear();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains false after shrinkToFit()") {
    timebomb.setCountdown(2);
    doc.add(0);
    timebomb.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains true after shrinkToFit()") {
    timebomb.setCountdown(0);
    doc.add(0);
    timebomb.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false when string length doesn't overflow") {
    auto maxLength = ArduinoJson::detail::StringNode::maxLength;
    CHECK(doc.set(std::string(maxLength, 'a')) == true);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true when string length overflows") {
    auto maxLength = ArduinoJson::detail::StringNode::maxLength;
    CHECK(doc.set(std::string(maxLength + 1, 'a')) == false);
    CHECK(doc.overflowed() == true);
  }
}
