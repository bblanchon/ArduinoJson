#define ARDUINOJSON_ENABLE_INFINITY 1
#include <ArduinoJson.h>

#include <catch.hpp>
#include <limits>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

TEST_CASE("ARDUINOJSON_ENABLE_INFINITY == 1") {
  DynamicJsonDocument doc(4096);

  SECTION("serializeJson()") {
    doc.add(std::numeric_limits<double>::infinity());
    doc.add(-std::numeric_limits<double>::infinity());

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "[Infinity,-Infinity]");
  }

  SECTION("deserializeJson()") {
    DeserializationError err =
        deserializeJson(doc, "[Infinity,-Infinity,+Infinity]");
    float a = doc[0];
    float b = doc[1];
    float c = doc[2];

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(my::isinf(a));
    REQUIRE(a > 0);
    REQUIRE(my::isinf(b));
    REQUIRE(b < 0);
    REQUIRE(my::isinf(c));
    REQUIRE(c > 0);
  }
}
