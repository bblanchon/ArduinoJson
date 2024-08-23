// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonObject::operator[]") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("int") {
    obj["hello"] = 123;

    REQUIRE(123 == obj["hello"].as<int>());
    REQUIRE(true == obj["hello"].is<int>());
    REQUIRE(false == obj["hello"].is<bool>());
  }

  SECTION("volatile int") {  // issue #415
    volatile int i = 123;
    obj["hello"] = i;

    REQUIRE(123 == obj["hello"].as<int>());
    REQUIRE(true == obj["hello"].is<int>());
    REQUIRE(false == obj["hello"].is<bool>());
  }

  SECTION("double") {
    obj["hello"] = 123.45;

    REQUIRE(true == obj["hello"].is<double>());
    REQUIRE(false == obj["hello"].is<long>());
    REQUIRE(123.45 == obj["hello"].as<double>());
  }

  SECTION("bool") {
    obj["hello"] = true;

    REQUIRE(true == obj["hello"].is<bool>());
    REQUIRE(false == obj["hello"].is<long>());
    REQUIRE(true == obj["hello"].as<bool>());
  }

  SECTION("const char*") {
    obj["hello"] = "h3110";

    REQUIRE(true == obj["hello"].is<const char*>());
    REQUIRE(false == obj["hello"].is<long>());
    REQUIRE("h3110"_s == obj["hello"].as<const char*>());
  }

  SECTION("array") {
    JsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();

    obj["hello"] = arr;

    REQUIRE(arr == obj["hello"].as<JsonArray>());
    REQUIRE(true == obj["hello"].is<JsonArray>());
    REQUIRE(false == obj["hello"].is<JsonObject>());
  }

  SECTION("object") {
    JsonDocument doc2;
    JsonObject obj2 = doc2.to<JsonObject>();

    obj["hello"] = obj2;

    REQUIRE(obj2 == obj["hello"].as<JsonObject>());
    REQUIRE(true == obj["hello"].is<JsonObject>());
    REQUIRE(false == obj["hello"].is<JsonArray>());
  }

  SECTION("array subscript") {
    JsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();
    arr.add(42);

    obj["a"] = arr[0];

    REQUIRE(42 == obj["a"]);
  }

  SECTION("object subscript") {
    JsonDocument doc2;
    JsonObject obj2 = doc2.to<JsonObject>();
    obj2["x"] = 42;

    obj["a"] = obj2["x"];

    REQUIRE(42 == obj["a"]);
  }

  SECTION("char key[]") {  // issue #423
    char key[] = "hello";
    obj[key] = 42;
    REQUIRE(42 == obj[key]);
  }

  SECTION("should not duplicate const char*") {
    obj["hello"] = "world";
    REQUIRE(spy.log() == AllocatorLog{Allocate(sizeofPool())});
  }

  SECTION("should duplicate char* value") {
    obj["hello"] = const_cast<char*>("world");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate char* key") {
    obj[const_cast<char*>("hello")] = "world";
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("should duplicate char* key&value") {
    obj[const_cast<char*>("hello")] = const_cast<char*>("world");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate std::string value") {
    obj["hello"] = "world"_s;
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate std::string key") {
    obj["hello"_s] = "world";
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("should duplicate std::string key&value") {
    obj["hello"_s] = "world"_s;
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate a non-static JsonString key") {
    obj[JsonString("hello", JsonString::Copied)] = "world";
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("should not duplicate a static JsonString key") {
    obj[JsonString("hello", JsonString::Linked)] = "world";
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("should ignore null key") {
    // object must have a value to make a call to strcmp()
    obj["dummy"] = 42;

    const char* null = 0;
    obj[null] = 666;

    REQUIRE(obj.size() == 1);
    REQUIRE(obj[null] == null);
  }

  SECTION("obj[key].to<JsonArray>()") {
    JsonArray arr = obj["hello"].to<JsonArray>();

    REQUIRE(arr.isNull() == false);
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("obj[VLA] = str") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    obj[vla] = "world";

    REQUIRE("world"_s == obj["hello"]);
  }

  SECTION("obj[str] = VLA") {  // issue #416
    size_t i = 32;
    char vla[i];
    strcpy(vla, "world");

    obj["hello"] = vla;

    REQUIRE("world"_s == obj["hello"].as<const char*>());
  }

  SECTION("obj.set(VLA, str)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    obj[vla] = "world";

    REQUIRE("world"_s == obj["hello"]);
  }

  SECTION("obj.set(str, VLA)") {
    size_t i = 32;
    char vla[i];
    strcpy(vla, "world");

    obj["hello"].set(vla);

    REQUIRE("world"_s == obj["hello"].as<const char*>());
  }

  SECTION("obj[VLA]") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    deserializeJson(doc, "{\"hello\":\"world\"}");

    obj = doc.as<JsonObject>();
    REQUIRE("world"_s == obj[vla]);
  }
#endif

  SECTION("chain") {
    obj["hello"]["world"] = 123;

    REQUIRE(123 == obj["hello"]["world"].as<int>());
    REQUIRE(true == obj["hello"]["world"].is<int>());
    REQUIRE(false == obj["hello"]["world"].is<bool>());
  }

  SECTION("JsonVariant") {
    obj["hello"] = "world";
    doc["key"] = "hello";

    REQUIRE(obj[obj["key"]] == "world");
    REQUIRE(obj[obj["foo"]] == nullptr);
  }
}
