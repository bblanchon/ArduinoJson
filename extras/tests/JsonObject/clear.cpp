// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObject::clear()") {
  SUBCASE("No-op on null JsonObject") {
    JsonObject obj;
    obj.clear();
    REQUIRE(obj.isNull() == true);
    REQUIRE(obj.size() == 0);
  }

  SUBCASE("Removes all elements") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = 1;
    obj["world"] = 2;
    obj.clear();
    REQUIRE(obj.size() == 0);
    REQUIRE(obj.isNull() == false);
  }
}
