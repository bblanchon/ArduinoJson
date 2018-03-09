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

  //   JsonError err = deserializeJson(arr, json);

  //   REQUIRE(err != JsonError::Ok);
  // }

  // TODO: uncomment
  // SECTION("parseObject()") {
  //   char json[] = "{[]}";
  //   DynamicJsonObject obj;

  //   JsonError err = deserializeJson(obj, json);

  //   REQUIRE(err != JsonError::Ok);
  // }

  SECTION("startString()") {
    DynamicJsonBufferBase<NoMemoryAllocator>::String str =
        _jsonBuffer.startString();
    str.append('!');
    REQUIRE(0 == str.c_str());
  }
}
