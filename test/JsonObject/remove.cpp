// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::remove()") {
  DynamicJsonDocument doc;
  JsonObject& obj = doc.to<JsonObject>();

  SECTION("SizeDecreased_WhenValuesAreRemoved") {
    obj["hello"] = 1;

    obj.remove("hello");

    REQUIRE(0 == obj.size());
  }

  SECTION("SizeUntouched_WhenRemoveIsCalledWithAWrongKey") {
    obj["hello"] = 1;

    obj.remove("world");

    REQUIRE(1 == obj.size());
  }

  SECTION("RemoveByIterator") {
    obj["a"] = 0;
    obj["b"] = 1;
    obj["c"] = 2;

    for (JsonObject::iterator it = obj.begin(); it != obj.end(); ++it) {
      if (it->value == 1) obj.remove(it);
    }

    std::string result;
    serializeJson(obj, result);
    REQUIRE("{\"a\":0,\"c\":2}" == result);
  }
}
