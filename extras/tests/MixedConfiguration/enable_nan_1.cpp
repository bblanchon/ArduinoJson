#define ARDUINOJSON_ENABLE_NAN 1
#include <ArduinoJson.h>

#include <doctest.h>
#include <limits>

namespace my {
using ArduinoJson::detail::isnan;
}  // namespace my

TEST_CASE("ARDUINOJSON_ENABLE_NAN == 1") {
  JsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  SUBCASE("serializeJson()") {
    root["X"] = std::numeric_limits<double>::signaling_NaN();

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"X\":NaN}");
  }

  SUBCASE("deserializeJson()") {
    DeserializationError err = deserializeJson(doc, "{\"X\":NaN}");
    float x = doc["X"];

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(my::isnan(x));
  }
}
