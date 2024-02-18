// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JSON_ARRAY_SIZE") {
  REQUIRE(JSON_ARRAY_SIZE(10) == ArduinoJson::detail::sizeofArray(10));
}

TEST_CASE("JSON_OBJECT_SIZE") {
  REQUIRE(JSON_OBJECT_SIZE(10) == ArduinoJson::detail::sizeofObject(10));
}

TEST_CASE("JSON_STRING_SIZE") {
  REQUIRE(JSON_STRING_SIZE(10) == 11);  // issue #2054
}
