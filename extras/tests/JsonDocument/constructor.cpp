// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument constructor") {
  SpyingAllocator spyingAllocator;

  SUBCASE("JsonDocument(size_t)") {
    { JsonDocument doc(&spyingAllocator); }
    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SUBCASE("JsonDocument(const JsonDocument&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set("The size of this string is 32!!"_s);

      JsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                         Allocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                     });
  }

  SUBCASE("JsonDocument(JsonDocument&&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set("The size of this string is 32!!"_s);

      JsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");

      // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                     });
  }

  SUBCASE("JsonDocument(JsonObject, Allocator*)") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                     });
  }

  SUBCASE("JsonDocument(JsonObject)") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SUBCASE("JsonDocument(JsonArray, Allocator*)") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                     });
  }

  SUBCASE("JsonDocument(JsonArray)") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
  }

  SUBCASE("JsonDocument(JsonVariant, Allocator*)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>(), &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "hello");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello")),
                                     });
  }

  SUBCASE("JsonDocument(JsonVariant)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>());

    REQUIRE(doc2.as<std::string>() == "hello");
  }

  SUBCASE("JsonDocument(JsonVariantConst)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariantConst>());

    REQUIRE(doc2.as<std::string>() == "hello");
  }

  SUBCASE("JsonDocument(ElementProxy)") {
    JsonDocument doc1;
    deserializeJson(doc1, "[\"hello\",\"world\"]");

    JsonDocument doc2(doc1[1]);

    REQUIRE(doc2.as<std::string>() == "world");
  }

  SUBCASE("JsonDocument(MemberProxy)") {
    JsonDocument doc1;
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    JsonDocument doc2(doc1["hello"]);

    REQUIRE(doc2.as<std::string>() == "world");
  }
}
