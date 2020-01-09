// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StringBuilder") {
  char buffer[4096];

  SECTION("Works when buffer is big enough") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(6)));

    StringBuilder str(&pool);
    str.append("hello");

    REQUIRE(str.complete() == std::string("hello"));
  }

  SECTION("Returns null when too small") {
    MemoryPool pool(buffer, sizeof(void*));

    StringBuilder str(&pool);
    str.append("hello world!");

    REQUIRE(str.complete() == 0);
  }

  SECTION("Increases size of memory pool") {
    MemoryPool pool(buffer, addPadding(JSON_STRING_SIZE(6)));

    StringBuilder str(&pool);
    str.append('h');
    str.complete();

    REQUIRE(JSON_STRING_SIZE(2) == pool.size());
  }
}
