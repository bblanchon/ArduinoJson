// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

TEST_CASE("operator<<(std::ostream)") {
  JsonDocument doc;
  std::ostringstream os;

  SECTION("JsonVariant containing false") {
    JsonVariant variant = doc.to<JsonVariant>();

    variant.set(false);
    os << variant;

    REQUIRE("false" == os.str());
  }

  SECTION("JsonVariant containing string") {
    JsonVariant variant = doc.to<JsonVariant>();

    variant.set("coucou");
    os << variant;

    REQUIRE("\"coucou\"" == os.str());
  }

  SECTION("JsonObject") {
    JsonObject object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object;

    REQUIRE("{\"key\":\"value\"}" == os.str());
  }

  SECTION("MemberProxy") {
    JsonObject object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object["key"];

    REQUIRE("\"value\"" == os.str());
  }

  SECTION("JsonArray") {
    JsonArray array = doc.to<JsonArray>();
    array.add("value");

    os << array;

    REQUIRE("[\"value\"]" == os.str());
  }

  SECTION("ElementProxy") {
    JsonArray array = doc.to<JsonArray>();
    array.add("value");

    os << array[0];

    REQUIRE("\"value\"" == os.str());
  }
}
