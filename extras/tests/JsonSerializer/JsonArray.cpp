// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

static void check(JsonArray array, std::string expected) {
  std::string actual;
  size_t actualLen = serializeJson(array, actual);
  REQUIRE(expected == actual);
  REQUIRE(actualLen == expected.size());
  size_t measuredLen = measureJson(array);
  REQUIRE(measuredLen == expected.size());
}

TEST_CASE("serializeJson(JsonArray)") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();

  SUBCASE("Empty") {
    check(array, "[]");
  }

  SUBCASE("Null") {
    array.add(static_cast<char*>(0));

    check(array, "[null]");
  }

  SUBCASE("OneString") {
    array.add("hello");

    check(array, "[\"hello\"]");
  }

  SUBCASE("TwoStrings") {
    array.add("hello");
    array.add("world");

    check(array, "[\"hello\",\"world\"]");
  }

  SUBCASE("One double") {
    array.add(3.1415927);
    check(array, "[3.1415927]");
  }

  SUBCASE("OneInteger") {
    array.add(1);

    check(array, "[1]");
  }

  SUBCASE("TwoIntegers") {
    array.add(1);
    array.add(2);

    check(array, "[1,2]");
  }

  SUBCASE("serialized(const char*)") {
    array.add(serialized("{\"key\":\"value\"}"));

    check(array, "[{\"key\":\"value\"}]");
  }

  SUBCASE("serialized(char*)") {
    char tmp[] = "{\"key\":\"value\"}";
    array.add(serialized(tmp));

    check(array, "[{\"key\":\"value\"}]");
  }

  SUBCASE("OneTrue") {
    array.add(true);

    check(array, "[true]");
  }

  SUBCASE("OneFalse") {
    array.add(false);

    check(array, "[false]");
  }

  SUBCASE("TwoBooleans") {
    array.add(false);
    array.add(true);

    check(array, "[false,true]");
  }

  SUBCASE("OneEmptyNestedArray") {
    array.add<JsonArray>();

    check(array, "[[]]");
  }

  SUBCASE("OneEmptyNestedHash") {
    array.add<JsonObject>();

    check(array, "[{}]");
  }
}
