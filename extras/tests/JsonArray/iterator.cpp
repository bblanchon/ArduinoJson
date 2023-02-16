// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename TArray>
static void run_iterator_test() {
  StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
  JsonArray tmp = doc.to<JsonArray>();
  tmp.add(12);
  tmp.add(34);

  TArray array = tmp;
  typename TArray::iterator it = array.begin();
  typename TArray::iterator end = array.end();

  REQUIRE(end != it);
  REQUIRE(12 == it->template as<int>());
  REQUIRE(12 == static_cast<int>(*it));
  ++it;
  REQUIRE(end != it);
  REQUIRE(34 == it->template as<int>());
  REQUIRE(34 == static_cast<int>(*it));
  ++it;
  REQUIRE(end == it);
}

TEST_CASE("JsonArray::begin()/end()") {
  SECTION("Non null JsonArray") {
    run_iterator_test<JsonArray>();
  }

  SECTION("Null JsonArray") {
    JsonArray array;

    REQUIRE(array.begin() == array.end());
  }
}

TEST_CASE("JsonArrayConst::begin()/end()") {
  SECTION("Non null JsonArrayConst") {
    run_iterator_test<JsonArrayConst>();
  }

  SECTION("Null JsonArrayConst") {
    JsonArrayConst array;

    REQUIRE(array.begin() == array.end());
  }
}
