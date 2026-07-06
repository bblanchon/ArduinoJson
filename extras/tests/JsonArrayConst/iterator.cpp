// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArrayConst::begin()/end()") {
  SUBCASE("Non null JsonArrayConst") {
    JsonDocument doc;
    JsonArrayConst array = doc.to<JsonArray>();
    doc.add(12);
    doc.add(34);

    auto it = array.begin();
    auto end = array.end();

    REQUIRE(end != it);
    REQUIRE(12 == it->as<int>());
    REQUIRE(12 == static_cast<int>(*it));
    ++it;
    REQUIRE(end != it);
    REQUIRE(34 == it->as<int>());
    REQUIRE(34 == static_cast<int>(*it));
    ++it;
    REQUIRE(end == it);
  }

  SUBCASE("Null JsonArrayConst") {
    JsonArrayConst array;

    REQUIRE(array.begin() == array.end());
  }
}
