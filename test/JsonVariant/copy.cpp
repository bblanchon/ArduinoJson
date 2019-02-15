// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant copy") {
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _variant1;
  JsonVariant _variant2;

  SECTION("IntegersAreCopiedByValue") {
    _variant1 = 123;
    _variant2 = _variant1;
    _variant1 = 456;

    REQUIRE(123 == _variant2.as<int>());
  }

  SECTION("DoublesAreCopiedByValue") {
    _variant1 = 123.45;
    _variant2 = _variant1;
    _variant1 = 456.78;

    REQUIRE(123.45 == _variant2.as<double>());
  }

  SECTION("BooleansAreCopiedByValue") {
    _variant1 = true;
    _variant2 = _variant1;
    _variant1 = false;

    REQUIRE(_variant2.as<bool>());
  }

  SECTION("StringsAreCopiedByValue") {
    _variant1 = "hello";
    _variant2 = _variant1;
    _variant1 = "world";

    REQUIRE(std::string("hello") == _variant2.as<const char *>());
  }

  SECTION("ObjectsAreCopiedByReference") {
    JsonObject &object = _jsonBuffer.createObject();

    _variant1 = object;

    object["hello"] = "world";

    REQUIRE(1 == _variant1.as<JsonObject>().size());
  }

  SECTION("ArraysAreCopiedByReference") {
    JsonArray &array = _jsonBuffer.createArray();

    _variant1 = array;

    array.add("world");

    REQUIRE(1 == _variant1.as<JsonArray>().size());
  }
}
