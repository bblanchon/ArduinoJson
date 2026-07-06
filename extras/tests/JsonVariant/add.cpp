// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonVariant::add(T)") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SUBCASE("add integer to new variant") {
    var.add(42);

    REQUIRE(var.as<std::string>() == "[42]");
  }

  SUBCASE("add const char* to new variant") {
    var.add("hello");

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }

  SUBCASE("add std::string to new variant") {
    var.add("hello"_s);

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }

  SUBCASE("add integer to integer") {
    var.set(123);

    var.add(456);  // no-op

    REQUIRE(var.as<std::string>() == "123");
  }

  SUBCASE("add integer to object") {
    var["val"] = 123;

    var.add(456);  // no-op

    REQUIRE(var.as<std::string>() == "{\"val\":123}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLAs") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    var.add(vla);

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }
#endif
}

TEST_CASE("JsonVariant::add<T>()") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SUBCASE("JsonArray") {
    JsonArray array = var.add<JsonArray>();
    array.add(1);
    array.add(2);
    REQUIRE(doc.as<std::string>() == "[[1,2]]");
  }

  SUBCASE("JsonVariant") {
    JsonVariant variant = var.add<JsonVariant>();
    variant.set(42);
    REQUIRE(doc.as<std::string>() == "[42]");
  }
}

TEST_CASE("JsonObject::add(JsonObject) ") {
  JsonDocument doc1;
  doc1["hello"_s] = "world"_s;

  TimebombAllocator allocator(10);
  SpyingAllocator spy(&allocator);
  JsonDocument doc2(&spy);
  JsonVariant variant = doc2.to<JsonVariant>();

  SUBCASE("success") {
    bool result = variant.add(doc1.as<JsonObject>());

    REQUIRE(result == true);
    REQUIRE(doc2.as<std::string>() == "[{\"hello\":\"world\"}]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofString("world")),
                         });
  }

  SUBCASE("partial failure") {  // issue #2081
    allocator.setCountdown(2);

    bool result = variant.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             AllocateFail(sizeofString("world")),
                             Deallocate(sizeofString("hello")),
                         });
  }

  SUBCASE("complete failure") {
    allocator.setCountdown(0);

    bool result = variant.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofPool()),
                         });
  }
}
