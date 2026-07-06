#define ARDUINOJSON_ENABLE_INFINITY 1
#include <ArduinoJson.h>

#include <doctest.h>
#include <limits>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

TEST_CASE("ARDUINOJSON_ENABLE_INFINITY == 1") {
  JsonDocument doc;

  SUBCASE("serializeJson()") {
    doc.add(std::numeric_limits<double>::infinity());
    doc.add(-std::numeric_limits<double>::infinity());

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "[Infinity,-Infinity]");
  }

  SUBCASE("deserializeJson()") {
    DeserializationError err =
        deserializeJson(doc, "[Infinity,-Infinity,+Infinity]");
    float a = doc[0];
    float b = doc[1];
    float c = doc[2];

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(my::isinf(a));
    REQUIRE(a > 0.0f);
    REQUIRE(my::isinf(b));
    REQUIRE(b < 0.0f);
    REQUIRE(my::isinf(c));
    REQUIRE(c > 0.0f);
  }
}
