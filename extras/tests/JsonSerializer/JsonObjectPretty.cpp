// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

static void checkObjectPretty(const JsonObject obj,
                              const std::string expected) {
  char json[256];

  size_t actualLen = serializeJsonPretty(obj, json);
  size_t measuredLen = measureJsonPretty(obj);

  REQUIRE(json == expected);
  REQUIRE(expected.size() == actualLen);
  REQUIRE(expected.size() == measuredLen);
}

TEST_CASE("serializeJsonPretty(JsonObject)") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("EmptyObject") {
    checkObjectPretty(obj, "{}");
  }

  SECTION("OneMember") {
    obj["key"] = "value";

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key\": \"value\"\r\n"
                      "}");
  }

  SECTION("TwoMembers") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": \"value1\",\r\n"
                      "  \"key2\": \"value2\"\r\n"
                      "}");
  }

  SECTION("EmptyNestedContainers") {
    obj["key1"].to<JsonObject>();
    obj["key2"].to<JsonArray>();

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": {},\r\n"
                      "  \"key2\": []\r\n"
                      "}");
  }

  SECTION("NestedContainers") {
    JsonObject nested1 = obj["key1"].to<JsonObject>();
    nested1["a"] = 1;

    JsonArray nested2 = obj["key2"].to<JsonArray>();
    nested2.add(2);

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": {\r\n"
                      "    \"a\": 1\r\n"
                      "  },\r\n"
                      "  \"key2\": [\r\n"
                      "    2\r\n"
                      "  ]\r\n"
                      "}");
  }
}
