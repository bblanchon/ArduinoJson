// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

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

  SECTION("Duplicates different strings") {
    auto a = saveString(resources, "hello");
    auto b = saveString(resources, "world");
    REQUIRE(+a->data != +b->data);
    REQUIRE(a->length == 5);
    REQUIRE(b->length == 5);
    REQUIRE(a->references == 1);
    REQUIRE(b->references == 1);
    REQUIRE(resources.size() == sizeofString("hello") + sizeofString("world"));
  }

  SECTION("Deduplicates identical strings") {
    auto a = saveString(resources, "hello");
    auto b = saveString(resources, "hello");
    REQUIRE(a == b);
    REQUIRE(a->length == 5);
    REQUIRE(a->references == 2);
    REQUIRE(resources.size() == sizeofString("hello"));
  }

  SECTION("Deduplicates identical strings that contain NUL") {
    auto a = saveString(resources, "hello\0world", 11);
    auto b = saveString(resources, "hello\0world", 11);
    REQUIRE(a == b);
    REQUIRE(a->length == 11);
    REQUIRE(a->references == 2);
    REQUIRE(resources.size() == sizeofString("hello world"));
  }

  SECTION("Don't stop on first NUL") {
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

  SECTION("Returns NULL when allocation fails") {
    ResourceManager pool2(FailingAllocator::instance());
    REQUIRE(saveString(pool2, "a") == nullptr);
  }
}
