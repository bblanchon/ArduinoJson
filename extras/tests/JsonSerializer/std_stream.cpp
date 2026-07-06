// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <sstream>

TEST_CASE("operator<<(std::ostream)") {
  JsonDocument doc;
  std::ostringstream os;

  SUBCASE("JsonVariant containing false") {
    JsonVariant variant = doc.to<JsonVariant>();

    variant.set(false);
    os << variant;

    REQUIRE("false" == os.str());
  }

  SUBCASE("JsonVariant containing string") {
    JsonVariant variant = doc.to<JsonVariant>();

    variant.set("coucou");
    os << variant;

    REQUIRE("\"coucou\"" == os.str());
  }

  SUBCASE("JsonObject") {
    JsonObject object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object;

    REQUIRE("{\"key\":\"value\"}" == os.str());
  }

  SUBCASE("MemberProxy") {
    JsonObject object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object["key"];

    REQUIRE("\"value\"" == os.str());
  }

  SUBCASE("JsonArray") {
    JsonArray array = doc.to<JsonArray>();
    array.add("value");

    os << array;

    REQUIRE("[\"value\"]" == os.str());
  }

  SUBCASE("ElementProxy") {
    JsonArray array = doc.to<JsonArray>();
    array.add("value");

    os << array[0];

    REQUIRE("\"value\"" == os.str());
  }
}
