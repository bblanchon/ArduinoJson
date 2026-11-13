#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#include <catch.hpp>

using ArduinoJson::detail::NumberType;
using ArduinoJson::detail::parseNumber;

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 1") {
  SECTION("serializeJson()") {
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();

    root["A"] = 123456789123456789;
    root["B"] = 987654321987654321;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"A\":123456789123456789,\"B\":987654321987654321}");
  }

  SECTION("parseNumber()") {
    SECTION("uint64_t overflow") {
      auto num1 = parseNumber("18446744073709551615");
      auto num2 = parseNumber("18446744073709551616");

      REQUIRE(num1.type() == NumberType::UnsignedInteger);
      CHECK(num1.asUnsignedInteger() == 18446744073709551615ULL);

      REQUIRE(num2.type() == NumberType::Double);
      CHECK(num2.asDouble() == Approx(18446744073709551616.0));
    }

    SECTION("int64_t overflow") {
      auto num1 = parseNumber("-9223372036854775808");
      auto num2 = parseNumber("-9223372036854775809");

      REQUIRE(num1.type() == NumberType::SignedInteger);
      CHECK(num1.asSignedInteger() == -9223372036854775807 - 1);

      REQUIRE(num2.type() == NumberType::Double);
      CHECK(num2.asDouble() == Approx(-9223372036854775809.0));
    }
  }
}
