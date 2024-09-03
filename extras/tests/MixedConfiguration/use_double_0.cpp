#define ARDUINOJSON_USE_DOUBLE 0
#include <ArduinoJson.h>

#include <catch.hpp>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

void checkFloat(const char* input, float expected) {
  using ArduinoJson::detail::NumberType;
  using ArduinoJson::detail::parseNumber;
  CAPTURE(input);
  auto result = parseNumber(input);
  REQUIRE(result.type() == NumberType::Float);
  REQUIRE(result.asFloat() == Approx(expected));
}

TEST_CASE("ARDUINOJSON_USE_DOUBLE == 0") {
  SECTION("serializeJson()") {
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();

    root["pi"] = 3.14;
    root["e"] = 2.72;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"pi\":3.14,\"e\":2.72}");
  }

  SECTION("parseNumber()") {
    using ArduinoJson::detail::NumberType;
    using ArduinoJson::detail::parseNumber;

    SECTION("Large positive number") {
      auto result = parseNumber("1e300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() > 0);
      REQUIRE(my::isinf(result.asFloat()));
    }

    SECTION("Large negative number") {
      auto result = parseNumber("-1e300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() < 0);
      REQUIRE(my::isinf(result.asFloat()));
    }

    SECTION("Too small to be represented") {
      auto result = parseNumber("1e-300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() == 0);
    }

    SECTION("MantissaTooLongToFit") {
      checkFloat("0.340282346638528861111111111111", 0.34028234663852886f);
      checkFloat("34028234663852886.11111111111111", 34028234663852886.0f);
      checkFloat("34028234.66385288611111111111111", 34028234.663852886f);

      checkFloat("-0.340282346638528861111111111111", -0.34028234663852886f);
      checkFloat("-34028234663852886.11111111111111", -34028234663852886.0f);
      checkFloat("-34028234.66385288611111111111111", -34028234.663852886f);
    }
  }
}
