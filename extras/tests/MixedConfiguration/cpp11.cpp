#include <ArduinoJson.h>

#include <catch.hpp>

#if __cplusplus >= 201103L

TEST_CASE("nullptr") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("JsonVariant == nullptr") {
    REQUIRE((variant == nullptr));
    REQUIRE_FALSE((variant != nullptr));
  }

  SECTION("JsonVariant != nullptr") {
    variant.set(42);

    REQUIRE_FALSE((variant == nullptr));
    REQUIRE((variant != nullptr));
  }

  SECTION("JsonVariant.set(nullptr)") {
    variant.set(42);
    variant.set(nullptr);

    REQUIRE(variant.isNull());
  }
}

#endif
