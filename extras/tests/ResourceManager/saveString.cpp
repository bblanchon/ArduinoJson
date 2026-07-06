// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

static StringNode* saveString(ResourceManager& resources, const char* s) {
  return resources.saveString(adaptString(s));
}

static StringNode* saveString(ResourceManager& resources, const char* s,
                              size_t n) {
  return resources.saveString(adaptString(s, n));
}

TEST_CASE("ResourceManager::saveString()") {
  ResourceManager resources;

  SUBCASE("Duplicates different strings") {
    auto a = saveString(resources, "hello");
    auto b = saveString(resources, "world");
    REQUIRE(+a->data != +b->data);
    REQUIRE(a->length == 5);
    REQUIRE(b->length == 5);
    REQUIRE(a->references == 1);
    REQUIRE(b->references == 1);
    REQUIRE(resources.size() == sizeofString("hello") + sizeofString("world"));
  }

  SUBCASE("Deduplicates identical strings") {
    auto a = saveString(resources, "hello");
    auto b = saveString(resources, "hello");
    REQUIRE(a == b);
    REQUIRE(a->length == 5);
    REQUIRE(a->references == 2);
    REQUIRE(resources.size() == sizeofString("hello"));
  }

  SUBCASE("Deduplicates identical strings that contain NUL") {
    auto a = saveString(resources, "hello\0world", 11);
    auto b = saveString(resources, "hello\0world", 11);
    REQUIRE(a == b);
    REQUIRE(a->length == 11);
    REQUIRE(a->references == 2);
    REQUIRE(resources.size() == sizeofString("hello world"));
  }

  SUBCASE("Don't stop on first NUL") {
    auto a = saveString(resources, "hello");
    auto b = saveString(resources, "hello\0world", 11);
    REQUIRE(a != b);
    REQUIRE(a->length == 5);
    REQUIRE(b->length == 11);
    REQUIRE(a->references == 1);
    REQUIRE(b->references == 1);
    REQUIRE(resources.size() ==
            sizeofString("hello") + sizeofString("hello world"));
  }

  SUBCASE("Returns NULL when allocation fails") {
    ResourceManager pool2(FailingAllocator::instance());
    REQUIRE(saveString(pool2, "a") == nullptr);
  }
}
