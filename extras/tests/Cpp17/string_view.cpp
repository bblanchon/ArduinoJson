#include <ArduinoJson.h>

#include <catch.hpp>
#include <string_view>

#if !ARDUINOJSON_ENABLE_STRING_VIEW
#  error ARDUINOJSON_ENABLE_STRING_VIEW must be set to 1
#endif

TEST_CASE("string_view") {
  StaticJsonDocument<128> doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("deserializeJson()") {
    auto err = deserializeJson(doc, std::string_view("123", 2));
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<int>() == 12);
  }

  SECTION("JsonDocument::set()") {
    doc.set(std::string_view("123", 2));
    REQUIRE(doc.as<std::string>() == "12");
  }

  SECTION("JsonDocument::operator[]() const") {
    doc["ab"] = "Yes";
    doc["abc"] = "No";
    REQUIRE(doc[std::string_view("abc", 2)] == "Yes");
  }

  SECTION("JsonDocument::operator[]()") {
    doc[std::string_view("abc", 2)] = "Yes";
    REQUIRE(doc["ab"] == "Yes");
  }

  SECTION("JsonVariant::operator==()") {
    variant.set("A");
    REQUIRE(variant == std::string_view("AX", 1));
    REQUIRE_FALSE(variant == std::string_view("BX", 1));
  }

  SECTION("JsonVariant::operator>()") {
    variant.set("B");
    REQUIRE(variant > std::string_view("AX", 1));
    REQUIRE_FALSE(variant > std::string_view("CX", 1));
  }

  SECTION("JsonVariant::operator<()") {
    variant.set("B");
    REQUIRE(variant < std::string_view("CX", 1));
    REQUIRE_FALSE(variant < std::string_view("AX", 1));
  }

  SECTION("String deduplication") {
    doc.add(std::string_view("example one", 7));
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1) + 8);

    doc.add(std::string_view("example two", 7));
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2) + 8);
  }
}

using ARDUINOJSON_NAMESPACE::adaptString;

TEST_CASE("StringViewAdapter") {
  std::string_view str("bravoXXX", 5);
  auto adapter = adaptString(str);

  CHECK(adapter.compare(NULL) > 0);
  CHECK(adapter.compare("alpha") > 0);
  CHECK(adapter.compare("bravo") == 0);
  CHECK(adapter.compare("charlie") < 0);

  CHECK(adapter.equals("bravo"));
  CHECK_FALSE(adapter.equals("charlie"));
  CHECK_FALSE(adapter.equals(NULL));

  CHECK(adapter.size() == 5);
}
