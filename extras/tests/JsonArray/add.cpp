// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonArray::add(T)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonArray array = doc.to<JsonArray>();

  SECTION("int") {
    array.add(123);
    REQUIRE(123 == array[0].as<int>());
    REQUIRE(array[0].is<int>());
    REQUIRE(array[0].is<double>());
  }

  SECTION("double") {
    array.add(123.45);
    REQUIRE(123.45 == array[0].as<double>());
    REQUIRE(array[0].is<double>());
    REQUIRE_FALSE(array[0].is<bool>());
  }

  SECTION("bool") {
    array.add(true);
    REQUIRE(true == array[0].as<bool>());
    REQUIRE(array[0].is<bool>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("const char*") {
    const char* str = "hello";
    array.add(str);
    REQUIRE(str == array[0].as<std::string>());
    REQUIRE(array[0].is<const char*>());
    REQUIRE_FALSE(array[0].is<int>());
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("vla") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add(vla);

    REQUIRE("world"_s == array[0]);
  }
#endif

  SECTION("nested array") {
    JsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();

    array.add(arr);

    REQUIRE(arr == array[0].as<JsonArray>());
    REQUIRE(array[0].is<JsonArray>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("nested object") {
    JsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    array.add(obj);

    REQUIRE(obj == array[0].as<JsonObject>());
    REQUIRE(array[0].is<JsonObject>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("array subscript") {
    const char* str = "hello";
    JsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();
    arr.add(str);

    array.add(arr[0]);

    REQUIRE(str == array[0]);
  }

  SECTION("object subscript") {
    const char* str = "hello";
    JsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();
    obj["x"] = str;

    array.add(obj["x"]);

    REQUIRE(str == array[0]);
  }

  SECTION("should not duplicate const char*") {
    array.add("world");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("should duplicate char*") {
    array.add(const_cast<char*>("world"));
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate std::string") {
    array.add("world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should duplicate serialized(const char*)") {
    array.add(serialized("{}"));
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("{}")),
                         });
  }

  SECTION("should duplicate serialized(char*)") {
    array.add(serialized(const_cast<char*>("{}")));
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("{}")),
                         });
  }

  SECTION("should duplicate serialized(std::string)") {
    array.add(serialized("{}"_s));
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("{}")),
                         });
  }

  SECTION("should duplicate serialized(std::string)") {
    array.add(serialized("\0XX"_s));
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString(" XX")),
                         });
  }
}

TEST_CASE("JsonArray::add<T>()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();

  SECTION("add<JsonArray>()") {
    JsonArray nestedArray = array.add<JsonArray>();
    nestedArray.add(1);
    nestedArray.add(2);
    REQUIRE(doc.as<std::string>() == "[[1,2]]");
  }

  SECTION("add<JsonObject>()") {
    JsonObject nestedObject = array.add<JsonObject>();
    nestedObject["a"] = 1;
    nestedObject["b"] = 2;
    REQUIRE(doc.as<std::string>() == "[{\"a\":1,\"b\":2}]");
  }

  SECTION("add<JsonVariant>()") {
    JsonVariant nestedVariant = array.add<JsonVariant>();
    nestedVariant.set(42);
    REQUIRE(doc.as<std::string>() == "[42]");
  }
}

TEST_CASE("JsonObject::add(JsonObject) ") {
  JsonDocument doc1;
  doc1["key1"_s] = "value1"_s;

  TimebombAllocator allocator(10);
  SpyingAllocator spy(&allocator);
  JsonDocument doc2(&spy);
  JsonArray array = doc2.to<JsonArray>();

  SECTION("success") {
    bool result = array.add(doc1.as<JsonObject>());

    REQUIRE(result == true);
    REQUIRE(doc2.as<std::string>() == "[{\"key1\":\"value1\"}]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("key1")),
                             Allocate(sizeofString("value1")),
                         });
  }

  SECTION("partial failure") {  // issue #2081
    allocator.setCountdown(2);

    bool result = array.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("key1")),
                             AllocateFail(sizeofString("value1")),
                             Deallocate(sizeofString("key1")),
                         });
  }

  SECTION("complete failure") {
    allocator.setCountdown(0);

    bool result = array.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofPool()),
                         });
  }
}
