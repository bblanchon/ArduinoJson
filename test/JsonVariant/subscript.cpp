// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::operator[]") {
  SECTION("The JsonVariant is undefined") {
    JsonVariant var = JsonVariant();
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("The JsonVariant is a string") {
    JsonVariant var = "hello world";
    REQUIRE(0 == var.size());
    REQUIRE(var["0"].isNull());
    REQUIRE(var[0].isNull());
  }

  SECTION("The JsonVariant is a JsonArray") {
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    JsonVariant var = array;

    SECTION("get value") {
      array.add("element at index 0");
      array.add("element at index 1");

      REQUIRE(2 == var.size());
      REQUIRE(std::string("element at index 0") == var[0]);
      REQUIRE(std::string("element at index 1") == var[1]);
      REQUIRE(std::string("element at index 0") ==
              var[static_cast<unsigned char>(0)]);  // issue #381
      REQUIRE(var[666].isNull());
      REQUIRE(var[3].isNull());
      REQUIRE(var["0"].isNull());
    }

    SECTION("set value") {
      array.add("hello");

      var[0] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var[0]);
    }

    SECTION("set value in a nested object") {
      array.createNestedObject();

      var[0]["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(1 == var[0].size());
      REQUIRE(std::string("world") == var[0]["hello"]);
    }
  }

  SECTION("The JsonVariant is a JsonObject") {
    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();
    JsonVariant var = object;

    SECTION("get value") {
      object["a"] = "element at key \"a\"";
      object["b"] = "element at key \"b\"";

      REQUIRE(2 == var.size());
      REQUIRE(std::string("element at key \"a\"") == var["a"]);
      REQUIRE(std::string("element at key \"b\"") == var["b"]);
      REQUIRE(var["c"].isNull());
      REQUIRE(var[0].isNull());
    }

    SECTION("set value, key is a const char*") {
      var["hello"] = "world";

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var["hello"]);
    }

    SECTION("set value, key is a char[]") {
      char key[] = "hello";
      var[key] = "world";
      key[0] = '!';  // make sure the key is duplicated

      REQUIRE(1 == var.size());
      REQUIRE(std::string("world") == var["hello"]);
    }
  }
}
