// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <string>

TEST_CASE("JsonDocument::createNestedArray()") {
  JsonDocument doc;

  SECTION("createNestedArray()") {
    JsonArray array = doc.createNestedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createNestedArray(const char*)") {
    JsonArray array = doc.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = doc.createNestedArray(std::string("key"));
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = doc.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonArray::createNestedArray()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  JsonArray nestedArray = array.createNestedArray();
  nestedArray.add(42);
  REQUIRE(doc.as<std::string>() == "[[42]]");
}

TEST_CASE("JsonObject::createNestedArray()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();

  SECTION("createNestedArray(const char*)") {
    JsonArray array = object.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = object.createNestedArray(std::string("key"));
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = object.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonVariant::createNestedArray()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("createNestedArray()") {
    JsonArray array = variant.createNestedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createNestedArray(const char*)") {
    JsonArray array = variant.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = variant.createNestedArray(std::string("key"));
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = variant.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}
