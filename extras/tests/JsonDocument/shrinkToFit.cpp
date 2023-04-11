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
  virtual ~ArmoredAllocator() {}

  void* allocate(size_t size) override {
    return malloc(size);
  }

  void deallocate(void* ptr) override {
    free(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) override {
    // don't call realloc, instead alloc a new buffer and erase the old one
    // this way we make sure we support relocation
    void* new_ptr = malloc(new_size);
    memset(new_ptr, '#', new_size);  // erase
    memcpy(new_ptr, ptr, std::min(new_size, new_size));
    free(ptr);
    return new_ptr;
  }
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
    REQUIRE(doc.as<std::string>() == "abcdefg");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "abcdefg");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Allocate(sizeofString(7))
                           << AllocatorLog::Reallocate(4096, 0));
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
                           << AllocatorLog::Allocate(sizeofString(7))
                           << AllocatorLog::Reallocate(4096, 0));
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
                           << AllocatorLog::Allocate(sizeofString(7))
                           << AllocatorLog::Reallocate(4096, sizeofObject(1)));
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
                           << AllocatorLog::Allocate(sizeofString(7))
                           << AllocatorLog::Reallocate(4096, sizeofArray(1)));
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
                           << AllocatorLog::Allocate(sizeofString(7))
                           << AllocatorLog::Reallocate(4096, sizeofObject(1)));
  }
}
