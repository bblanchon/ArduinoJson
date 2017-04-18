// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

void check(const JsonObject &obj, const std::string &expected) {
  char actual[256];
  size_t actualLen = obj.printTo(actual);
  size_t measuredLen = obj.measureLength();

  REQUIRE(expected == actual);
  REQUIRE(expected.size() == actualLen);
  REQUIRE(expected.size() == measuredLen);
}
TEST_CASE("JsonObject::printTo()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject &obj = _jsonBuffer.createObject();

  SECTION("EmptyObject") {
    check(obj, "{}");
  }

  SECTION("TwoStrings") {
    obj["key1"] = "value1";
    obj.set("key2", "value2");

    check(obj, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
  }

  SECTION("RemoveFirst") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key1");

    check(obj, "{\"key2\":\"value2\"}");
  }

  SECTION("RemoveLast") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key2");

    check(obj, "{\"key1\":\"value1\"}");
  }

  SECTION("RemoveUnexistingKey") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";
    obj.remove("key3");

    check(obj, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
  }

  SECTION("ReplaceExistingKey") {
    obj["key"] = "value1";
    obj["key"] = "value2";

    check(obj, "{\"key\":\"value2\"}");
  }

  SECTION("TwoIntegers") {
    obj["a"] = 1;
    obj.set("b", 2);
    check(obj, "{\"a\":1,\"b\":2}");
  }

  SECTION("RawJson") {
    obj["a"] = RawJson("[1,2]");
    obj.set("b", RawJson("[4,5]"));
    check(obj, "{\"a\":[1,2],\"b\":[4,5]}");
  }

  SECTION("TwoDoublesFourDigits") {
    obj["a"] = double_with_n_digits(3.14159265358979323846, 4);
    obj.set("b", 2.71828182845904523536, 4);
    obj.set("c", double_with_n_digits(3.14159265358979323846, 3));
    check(obj, "{\"a\":3.1416,\"b\":2.7183,\"c\":3.142}");
  }

  SECTION("TwoDoubleDefaultDigits") {
    obj["a"] = 3.14159265358979323846;
    obj.set("b", 2.71828182845904523536);
    check(obj, "{\"a\":3.14,\"b\":2.72}");
  }

  SECTION("TwoNull") {
    obj["a"] = static_cast<char *>(0);
    obj.set("b", static_cast<char *>(0));
    check(obj, "{\"a\":null,\"b\":null}");
  }

  SECTION("TwoBooleans") {
    obj["a"] = true;
    obj.set("b", false);
    check(obj, "{\"a\":true,\"b\":false}");
  }

  SECTION("ThreeNestedArrays") {
    obj.createNestedArray("a");
    obj["b"] = _jsonBuffer.createArray();
    obj.set("c", _jsonBuffer.createArray());

    check(obj, "{\"a\":[],\"b\":[],\"c\":[]}");
  }

  SECTION("ThreeNestedObjects") {
    obj.createNestedObject("a");
    obj["b"] = _jsonBuffer.createObject();
    obj.set("c", _jsonBuffer.createObject());

    check(obj, "{\"a\":{},\"b\":{},\"c\":{}}");
  }
}
