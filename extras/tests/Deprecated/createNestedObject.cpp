// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

#include "Literals.hpp"

TEST_CASE("JsonDocument::createNestedObject()") {
  JsonDocument doc;

  SECTION("createNestedObject()") {
    JsonObject object = doc.createNestedObject();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("createNestedObject(const char*)") {
    JsonObject object = doc.createNestedObject("key");
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createNestedObject(std::string)") {
    JsonObject object = doc.createNestedObject("key"_s);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject object = doc.createNestedObject(vla);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}

TEST_CASE("JsonArray::createNestedObject()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  JsonObject object = array.createNestedObject();
  object["hello"] = "world";
  REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
}

TEST_CASE("JsonObject::createNestedObject()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();

  SECTION("createNestedObject(const char*)") {
    JsonObject nestedObject = object.createNestedObject("key");
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createNestedObject(std::string)") {
    JsonObject nestedObject = object.createNestedObject("key"_s);
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject nestedObject = object.createNestedObject(vla);
    nestedObject["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}

TEST_CASE("JsonVariant::createNestedObject()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("createNestedObject()") {
    JsonObject object = variant.createNestedObject();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("createNestedObject(const char*)") {
    JsonObject object = variant.createNestedObject("key");
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

  SECTION("createNestedObject(std::string)") {
    JsonObject object = variant.createNestedObject("key"_s);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedObject(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonObject object = variant.createNestedObject(vla);
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "{\"key\":{\"hello\":\"world\"}}");
  }
#endif
}
