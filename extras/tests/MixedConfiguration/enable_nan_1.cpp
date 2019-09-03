#define ARDUINOJSON_ENABLE_NAN 1
#include <ArduinoJson.h>

#include <catch.hpp>
#include <limits>

namespace my {
using ARDUINOJSON_NAMESPACE::isnan;
}  // namespace my

TEST_CASE("ARDUINOJSON_ENABLE_NAN == 1") {
  DynamicJsonDocument doc(4096);
  JsonObject root = doc.to<JsonObject>();

  SECTION("serializeJson()") {
    root["X"] = std::numeric_limits<double>::signaling_NaN();

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"X\":NaN}");
  }

  SECTION("deserializeJson()") {
    auto err = deserializeJson(doc, "{\"X\":NaN}");
    float x = doc["X"];

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(my::isnan(x));
  }
}
