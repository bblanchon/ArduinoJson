// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

// we expect ArduinoJson.h to include <string_view>
// but we don't want it to included accidentally
#undef ARDUINO
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#define ARDUINOJSON_ENABLE_STD_STRING 0

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

#if !ARDUINOJSON_ENABLE_STRING_VIEW
#  error ARDUINOJSON_ENABLE_STRING_VIEW must be set to 1
#endif

using ArduinoJson::detail::sizeofArray;

TEST_CASE("string_view") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("deserializeJson()") {
    auto err = deserializeJson(doc, std::string_view("123", 2));
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<int>() == 12);
  }

  SECTION("JsonDocument::set()") {
    doc.set(std::string_view("123", 2));
    REQUIRE(doc.as<std::string_view>() == "12");
  }

  SECTION("JsonDocument::operator[]() const") {
    doc["ab"] = "Yes";
    doc["abc"] = "No";
    REQUIRE(doc[std::string_view("abc", 2)] == "Yes");
  }

  SECTION("JsonDocument::operator[]()") {
    doc[std::string_view("abc", 2)] = "Yes";
    REQUIRE(doc["ab"] == "Yes");
  }

  SECTION("JsonVariant::operator==()") {
    variant.set("A");
    REQUIRE(variant == std::string_view("AX", 1));
    REQUIRE_FALSE(variant == std::string_view("BX", 1));
  }

  SECTION("JsonVariant::operator>()") {
    variant.set("B");
    REQUIRE(variant > std::string_view("AX", 1));
    REQUIRE_FALSE(variant > std::string_view("CX", 1));
  }

  SECTION("JsonVariant::operator<()") {
    variant.set("B");
    REQUIRE(variant < std::string_view("CX", 1));
    REQUIRE_FALSE(variant < std::string_view("AX", 1));
  }

  SECTION("String deduplication") {
    doc.add(std::string_view("example one", 7));
    doc.add(std::string_view("example two", 7));
    doc.add(std::string_view("example\0tree", 12));
    doc.add(std::string_view("example\0tree and a half", 12));

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                             Allocate(sizeofString("example tree")),
                         });
  }

  SECTION("as<std::string_view>()") {
    doc["s"] = "Hello World";
    doc["i"] = 42;
    REQUIRE(doc["s"].as<std::string_view>() == std::string_view("Hello World"));
    REQUIRE(doc["i"].as<std::string_view>() == std::string_view());
  }

  SECTION("is<std::string_view>()") {
    doc["s"] = "Hello World";
    doc["i"] = 42;
    REQUIRE(doc["s"].is<std::string_view>() == true);
    REQUIRE(doc["i"].is<std::string_view>() == false);
  }

  SECTION("String containing NUL") {
    doc.set(std::string("hello\0world", 11));
    REQUIRE(doc.as<std::string_view>().size() == 11);
    REQUIRE(doc.as<std::string_view>() == std::string_view("hello\0world", 11));
  }
}

using ArduinoJson::detail::adaptString;

TEST_CASE("StringViewAdapter") {
  std::string_view str("bravoXXX", 5);
  auto adapter = adaptString(str);

  CHECK(stringCompare(adapter, adaptString("alpha", 5)) > 0);
  CHECK(stringCompare(adapter, adaptString("bravo", 5)) == 0);
  CHECK(stringCompare(adapter, adaptString("charlie", 7)) < 0);

  CHECK(adapter.size() == 5);
}
