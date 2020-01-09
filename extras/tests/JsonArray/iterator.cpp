// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
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
  run_iterator_test<JsonArray>();
}

TEST_CASE("JsonArrayConst::begin()/end()") {
  run_iterator_test<JsonArrayConst>();
}
