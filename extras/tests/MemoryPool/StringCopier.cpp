// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StringCopier") {
  char buffer[4096];

  SECTION("Works when buffer is big enough") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(6)));
    StringCopier str;

    str.startString(&pool);
    str.append("hello");
    str.append('\0');

    REQUIRE(str.isValid() == true);
    REQUIRE(str.c_str() == std::string("hello"));
  }

  SECTION("Returns null when too small") {
    MemoryPool pool(buffer, sizeof(void*));
    StringCopier str;

    str.startString(&pool);
    str.append("hello world!");

    REQUIRE(str.isValid() == false);
  }

  SECTION("Increases size of memory pool") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(6)));
    StringCopier str;

    str.startString(&pool);
    str.append('h');
    str.save(&pool);

    REQUIRE(1 == pool.size());
  }
}

static const char* addStringToPool(MemoryPool* pool, const char* s) {
  StringCopier str;
  str.startString(pool);
  str.append(s);
  str.append('\0');
  return str.save(pool);
}

TEST_CASE("StringCopier::save() deduplicates strings") {
  char buffer[4096];
  MemoryPool pool(buffer, 4096);

  SECTION("Basic") {
    const char* s1 = addStringToPool(&pool, "hello");
    const char* s2 = addStringToPool(&pool, "world");
    const char* s3 = addStringToPool(&pool, "hello");

    REQUIRE(s1 == s3);
    REQUIRE(s2 != s3);
    REQUIRE(pool.size() == 12);
  }

  SECTION("Requires terminator") {
    const char* s1 = addStringToPool(&pool, "hello world");
    const char* s2 = addStringToPool(&pool, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == 12 + 6);
  }

  SECTION("Don't overrun") {
    const char* s1 = addStringToPool(&pool, "hello world");
    const char* s2 = addStringToPool(&pool, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == 12 + 4);
  }
}
