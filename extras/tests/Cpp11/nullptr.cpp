#include <ArduinoJson.h>

#include <catch.hpp>

#if !ARDUINOJSON_HAS_NULLPTR
#  error ARDUINOJSON_HAS_NULLPTR must be set to 1
#endif

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

  SECTION("JsonVariant.set(nullptr) with unbound reference") {
    JsonVariant unboundReference;

    unboundReference.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SECTION("JsonVariant.is<nullptr_t>()") {
    variant.set(42);
    REQUIRE(variant.is<std::nullptr_t>() == false);

    variant.clear();
    REQUIRE(variant.is<std::nullptr_t>() == true);
  }
}
