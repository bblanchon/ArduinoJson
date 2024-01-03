// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("VariantData") {
  REQUIRE(std::is_standard_layout<ArduinoJson::detail::VariantData>::value ==
          true);
}

TEST_CASE("StringNode") {
  REQUIRE(std::is_standard_layout<ArduinoJson::detail::StringNode>::value ==
          true);
}

TEST_CASE("JsonVariant from JsonArray") {
  SECTION("JsonArray is null") {
    JsonArray arr;
    JsonVariant v = arr;
    REQUIRE(v.isNull() == true);
  }

  SECTION("JsonArray is not null") {
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    arr.add(12);
    arr.add(34);

    JsonVariant v = arr;

    REQUIRE(v.is<JsonArray>() == true);
    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 12);
    REQUIRE(v[1] == 34);
  }
}

TEST_CASE("JsonVariant from JsonObject") {
  SECTION("JsonObject is null") {
    JsonObject obj;
    JsonVariant v = obj;
    REQUIRE(v.isNull() == true);
  }

  SECTION("JsonObject is not null") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    obj["a"] = 12;
    obj["b"] = 34;

    JsonVariant v = obj;

    REQUIRE(v.is<JsonObject>() == true);
    REQUIRE(v.size() == 2);
    REQUIRE(v["a"] == 12);
    REQUIRE(v["b"] == 34);
  }
}
