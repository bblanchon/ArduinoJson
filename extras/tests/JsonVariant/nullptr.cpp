#include <ArduinoJson.h>

#include <doctest.h>

TEST_CASE("nullptr") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SUBCASE("JsonVariant == nullptr") {
    REQUIRE(variant == nullptr);
    REQUIRE_FALSE(variant != nullptr);
  }

  SUBCASE("JsonVariant != nullptr") {
    variant.set(42);

    REQUIRE_FALSE(variant == nullptr);
    REQUIRE(variant != nullptr);
  }

  SUBCASE("JsonVariant.set(nullptr)") {
    variant.set(42);
    variant.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SUBCASE("JsonVariant.set(nullptr) with unbound reference") {
    JsonVariant unboundReference;

    unboundReference.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SUBCASE("JsonVariant.is<nullptr_t>()") {
    variant.set(42);
    REQUIRE(variant.is<std::nullptr_t>() == false);

    variant.clear();
    REQUIRE(variant.is<std::nullptr_t>() == true);
  }
}
