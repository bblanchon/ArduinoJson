// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

struct NoMemoryAllocator {
  void* allocate(size_t) {
    return NULL;
  }
  void deallocate(void*) {}
};

TEST_CASE("DynamicMemoryPool no memory") {
  DynamicMemoryPoolBase<NoMemoryAllocator> _memoryPool;

  typedef DynamicMemoryPoolBase<NoMemoryAllocator>::StringBuilder StringBuilder;

  SECTION("FixCodeCoverage") {
    // call this function to fix code coverage
    NoMemoryAllocator().deallocate(NULL);
  }

  // TODO: uncomment
  // SECTION("deserializeJson()") {
  //   char json[] = "{[]}";
  //   DynamicJsonDocument obj;

  //   DeserializationError err = deserializeJson(obj, json);

  //   REQUIRE(err != DeserializationError::Ok);
  // }

  SECTION("startString()") {
    StringBuilder str = _memoryPool.startString();
    str.append('!');
    REQUIRE(str.complete().isNull());
  }
}
