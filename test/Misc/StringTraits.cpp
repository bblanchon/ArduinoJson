// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

template <typename String>
bool should_duplicate() {
  return StringTraits<String>::should_duplicate;
}

TEST_CASE("StringTraits") {
  SECTION("should_duplicate") {
    REQUIRE(false == should_duplicate<const char*>());
    REQUIRE(true == should_duplicate<char*>());
    REQUIRE(true == should_duplicate<RawJsonString<char*> >());
    REQUIRE(false == should_duplicate<RawJsonString<const char*> >());
  }
}
