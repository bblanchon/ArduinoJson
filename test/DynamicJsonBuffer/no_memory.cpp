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

  SECTION("createArray()") {
    REQUIRE_FALSE(_jsonBuffer.createArray().success());
  }

  SECTION("createObject()") {
    REQUIRE_FALSE(_jsonBuffer.createObject().success());
  }

  SECTION("parseArray()") {
    char json[] = "[]";
    REQUIRE_FALSE(_jsonBuffer.parseArray(json).success());
  }

  SECTION("parseObject()") {
    char json[] = "{}";
    REQUIRE_FALSE(_jsonBuffer.parseObject(json).success());
  }

  SECTION("startString()") {
    DynamicJsonBufferBase<NoMemoryAllocator>::String str =
        _jsonBuffer.startString();
    str.append('!');
    REQUIRE(0 == str.c_str());
  }
}
