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
    str.commit(&pool);

    REQUIRE(1 == pool.size());
  }
}
