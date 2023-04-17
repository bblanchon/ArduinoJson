// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonVariant::clear()") {
  JsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("size goes back to zero") {
    var.add(42);
    var.clear();

    REQUIRE(var.size() == 0);
  }

  SECTION("isNull() return true") {
    var.add("hello");
    var.clear();

    REQUIRE(var.isNull() == true);
  }

  SECTION("releases owned string") {
    var.set(std::string("hello"));
    REQUIRE(doc.memoryUsage() == sizeofString(5));

    var.clear();
    REQUIRE(doc.memoryUsage() == 0);
  }
}
