// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <stdlib.h>  // malloc, free
#include <string>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;
using ArduinoJson::detail::sizeofString;

class ArmoredAllocator : public Allocator {
 public:
  ArmoredAllocator() : _ptr(0), _size(0) {}
  virtual ~ArmoredAllocator() {}

  void* allocate(size_t size) override {
    _ptr = malloc(size);
    _size = size;
    return _ptr;
  }

  void deallocate(void* ptr) override {
    REQUIRE(ptr == _ptr);
    free(ptr);
    _ptr = 0;
    _size = 0;
  }

  void* reallocate(void* ptr, size_t new_size) override {
    REQUIRE(ptr == _ptr);
    // don't call realloc, instead alloc a new buffer and erase the old one
    // this way we make sure we support relocation
    void* new_ptr = malloc(new_size);
    memcpy(new_ptr, _ptr, std::min(new_size, _size));
    memset(_ptr, '#', _size);  // erase
    free(_ptr);
    _ptr = new_ptr;
    return new_ptr;
  }

 private:
  void* _ptr;
  size_t _size;
};

TEST_CASE("JsonDocument::shrinkToFit()") {
  ArmoredAllocator armoredAllocator;
  SpyingAllocator spyingAllocator(&armoredAllocator);
  JsonDocument doc(4096, &spyingAllocator);

  SECTION("null") {
    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "null");
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Reallocate(4096, 0));
  }

  SECTION("empty object") {
    deserializeJson(doc, "{}");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofObject(0)));
  }

  SECTION("empty array") {
    deserializeJson(doc, "[]");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofArray(0)));
  }

  SECTION("linked string") {
    doc.set("hello");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "hello");
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Reallocate(4096, 0));
  }

  SECTION("owned string") {
    doc.set(std::string("abcdefg"));

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "abcdefg");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofString(7)));
  }

  SECTION("linked raw") {
    doc.set(serialized("[{},123]"));

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[{},123]");
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Reallocate(4096, 0));
  }

  SECTION("owned raw") {
    doc.set(serialized(std::string("[{},12]")));

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[{},12]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofString(7)));
  }

  SECTION("linked key") {
    doc["key"] = 42;

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"key\":42}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofObject(1)));
  }

  SECTION("owned key") {
    doc[std::string("abcdefg")] = 42;

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"abcdefg\":42}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(
                                  4096, sizeofObject(1) + sizeofString(7)));
  }

  SECTION("linked string in array") {
    doc.add("hello");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[\"hello\"]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofArray(1)));
  }

  SECTION("owned string in array") {
    doc.add(std::string("abcdefg"));

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[\"abcdefg\"]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(
                                  4096, sizeofArray(1) + sizeofString(7)));
  }

  SECTION("linked string in object") {
    doc["key"] = "hello";

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"key\":\"hello\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(4096, sizeofObject(1)));
  }

  SECTION("owned string in object") {
    doc["key"] = std::string("abcdefg");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"key\":\"abcdefg\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(
                                  4096, sizeofObject(1) + sizeofString(7)));
  }

  SECTION("unaligned") {
    doc.add(std::string("?"));  // two bytes in the string pool
    REQUIRE(doc.memoryUsage() == sizeofObject(1) + sizeofString(1));

    doc.shrinkToFit();

    // the new capacity should be padded to align the pointers
    REQUIRE(doc[0] == "?");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Reallocate(
                                  4096, sizeofArray(1) + sizeof(void*)));
  }
}
