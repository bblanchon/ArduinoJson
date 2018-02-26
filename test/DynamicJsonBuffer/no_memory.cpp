// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

struct NoMemoryAllocator {
  void* allocate(size_t) {
    return NULL;
  }
  void deallocate(void*) {}
};

TEST_CASE("DynamicJsonBuffer no memory") {
  DynamicJsonBufferBase<NoMemoryAllocator> _jsonBuffer;

  SECTION("FixCodeCoverage") {
    // call this function to fix code coverage
    NoMemoryAllocator().deallocate(NULL);
  }

  // TODO: uncomment
  // SECTION("parseArray()") {
  //   char json[] = "[{}]";
  //   DynamicJsonArray arr;

  //   bool success = deserializeJson(arr, json);

  //   REQUIRE(success == false);
  // }

  // TODO: uncomment
  // SECTION("parseObject()") {
  //   char json[] = "{[]}";
  //   DynamicJsonObject obj;

  //   bool success = deserializeJson(obj, json);

  //   REQUIRE(success == false);
  // }

  SECTION("startString()") {
    DynamicJsonBufferBase<NoMemoryAllocator>::String str =
        _jsonBuffer.startString();
    str.append('!');
    REQUIRE(0 == str.c_str());
  }
}
