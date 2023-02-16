// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("StringCopier") {
  char buffer[4096];

  SECTION("Works when buffer is big enough") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(5)));
    StringCopier str(&pool);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(pool.overflowed() == false);
  }

  SECTION("Returns null when too small") {
    MemoryPool pool(buffer, sizeof(void*));
    StringCopier str(&pool);

    str.startString();
    str.append("hello world!");

    REQUIRE(str.isValid() == false);
    REQUIRE(pool.overflowed() == true);
  }

  SECTION("Increases size of memory pool") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(6)));
    StringCopier str(&pool);

    str.startString();
    str.save();

    REQUIRE(1 == pool.size());
    REQUIRE(pool.overflowed() == false);
  }

  SECTION("Works when memory pool is 0 bytes") {
    MemoryPool pool(buffer, 0);
    StringCopier str(&pool);

    str.startString();
    REQUIRE(str.isValid() == false);
    REQUIRE(pool.overflowed() == true);
  }
}

static const char* addStringToPool(MemoryPool& pool, const char* s) {
  StringCopier str(&pool);
  str.startString();
  str.append(s);
  return str.save().c_str();
}

TEST_CASE("StringCopier::save() deduplicates strings") {
  char buffer[4096];
  MemoryPool pool(buffer, 4096);

  SECTION("Basic") {
    const char* s1 = addStringToPool(pool, "hello");
    const char* s2 = addStringToPool(pool, "world");
    const char* s3 = addStringToPool(pool, "hello");

    REQUIRE(s1 == s3);
    REQUIRE(s2 != s3);
    REQUIRE(pool.size() == 12);
  }

  SECTION("Requires terminator") {
    const char* s1 = addStringToPool(pool, "hello world");
    const char* s2 = addStringToPool(pool, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == 12 + 6);
  }

  SECTION("Don't overrun") {
    const char* s1 = addStringToPool(pool, "hello world");
    const char* s2 = addStringToPool(pool, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == 12 + 4);
  }
}
