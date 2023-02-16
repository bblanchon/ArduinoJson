// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::remove()") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("remove(int)") {
    var.add(1);
    var.add(2);
    var.add(3);

    var.remove(1);

    REQUIRE(var.as<std::string>() == "[1,3]");
  }

  SECTION("remove(const char *)") {
    var["a"] = 1;
    var["b"] = 2;

    var.remove("a");

    REQUIRE(var.as<std::string>() == "{\"b\":2}");
  }

  SECTION("remove(std::string)") {
    var["a"] = 1;
    var["b"] = 2;

    var.remove(std::string("b"));

    REQUIRE(var.as<std::string>() == "{\"a\":1}");
  }
}
