// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Unbound JsonVariant") {
  JsonVariant variant;

  SECTION("as<T>()") {
    CHECK(variant.as<bool>() == false);
    CHECK(variant.as<int>() == 0);
    CHECK(variant.as<float>() == 0.0f);
    CHECK(variant.as<const char*>() == 0);
    CHECK(variant.as<std::string>() == "null");
    CHECK(variant.as<JsonVariant>().isNull());
    CHECK(variant.as<JsonVariantConst>().isNull());
    CHECK(variant.as<JsonArray>().isNull());
    CHECK(variant.as<JsonArrayConst>().isNull());
    CHECK(variant.as<JsonObject>().isNull());
    CHECK(variant.as<JsonObjectConst>().isNull());
    CHECK(variant.as<JsonString>().isNull());
  }

  SECTION("is<T>()") {
    CHECK_FALSE(variant.is<bool>());
    CHECK_FALSE(variant.is<int>());
    CHECK_FALSE(variant.is<float>());
    CHECK_FALSE(variant.is<const char*>());
    CHECK_FALSE(variant.is<std::string>());
    CHECK_FALSE(variant.is<JsonVariant>());
    CHECK_FALSE(variant.is<JsonVariantConst>());
    CHECK_FALSE(variant.is<JsonArray>());
    CHECK_FALSE(variant.is<JsonArrayConst>());
    CHECK_FALSE(variant.is<JsonObject>());
    CHECK_FALSE(variant.is<JsonObjectConst>());
    CHECK_FALSE(variant.is<JsonString>());
  }

  SECTION("set()") {
    CHECK_FALSE(variant.set("42"));
    CHECK_FALSE(variant.set(42.0));
    CHECK_FALSE(variant.set(42L));
    CHECK_FALSE(variant.set(42U));
    CHECK_FALSE(variant.set(serialized("42")));
    CHECK_FALSE(variant.set(serialized(std::string("42"))));
    CHECK_FALSE(variant.set(true));
  }

  SECTION("add()") {
    CHECK_FALSE(variant.add("42"));
    CHECK_FALSE(variant.add(42.0));
    CHECK_FALSE(variant.add(42L));
    CHECK_FALSE(variant.add(42U));
    CHECK_FALSE(variant.add(serialized("42")));
    CHECK_FALSE(variant.add(true));
  }

  SECTION("operator[]") {
    CHECK(variant[0].isNull());
    CHECK(variant["key"].isNull());
    CHECK_FALSE(variant[0].set(1));
    CHECK_FALSE(variant["key"].set(1));
    CHECK_FALSE(variant[std::string("key")].set(1));
  }

  SECTION("containsKey()") {
    CHECK_FALSE(variant.containsKey("hello"));
  }

  SECTION("remove()") {
    variant.remove(0);
    variant.remove("hello");
  }
}
