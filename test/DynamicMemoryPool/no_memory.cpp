// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
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

  SECTION("StringBuilder returns null") {
    StringBuilder str(&_memoryPool);
    str.append('!');
    REQUIRE(str.complete().isNull());
  }
}
