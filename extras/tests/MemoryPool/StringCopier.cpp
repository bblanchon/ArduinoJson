// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("StringCopier") {
  SECTION("Works when buffer is big enough") {
    MemoryPool pool(addPadding(sizeofString(5)));
    StringCopier str(&pool);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(pool.overflowed() == false);
  }

  SECTION("Returns null when too small") {
    MemoryPool pool(sizeof(void*));
    StringCopier str(&pool);

    str.startString();
    str.append("hello world!");

    REQUIRE(str.isValid() == false);
    REQUIRE(pool.overflowed() == true);
  }

  SECTION("Increases size of memory pool") {
    MemoryPool pool(addPadding(sizeofString(6)));
    StringCopier str(&pool);

    str.startString();
    str.save();

    REQUIRE(1 == pool.size());
    REQUIRE(pool.overflowed() == false);
  }

  SECTION("Works when memory pool is 0 bytes") {
    MemoryPool pool(0);
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
  MemoryPool pool(4096);

  SECTION("Basic") {
    const char* s1 = addStringToPool(pool, "hello");
    const char* s2 = addStringToPool(pool, "world");
    const char* s3 = addStringToPool(pool, "hello");

    REQUIRE(s1 == s3);
    REQUIRE(s2 != s3);
    REQUIRE(pool.size() == 2 * sizeofString(5));
  }

  SECTION("Requires terminator") {
    const char* s1 = addStringToPool(pool, "hello world");
    const char* s2 = addStringToPool(pool, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == sizeofString(11) + sizeofString(5));
  }

  SECTION("Don't overrun") {
    const char* s1 = addStringToPool(pool, "hello world");
    const char* s2 = addStringToPool(pool, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(pool.size() == sizeofString(11) + sizeofString(3));
  }
}
