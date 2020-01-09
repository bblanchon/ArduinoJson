// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant and strings") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("stores const char* by reference") {
    char str[16];

    strcpy(str, "hello");
    variant.set(static_cast<const char *>(str));
    strcpy(str, "world");

    REQUIRE(variant == "world");
  }

  SECTION("stores char* by copy") {
    char str[16];

    strcpy(str, "hello");
    variant.set(str);
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }

  SECTION("stores unsigned char* by copy") {
    char str[16];

    strcpy(str, "hello");
    variant.set(reinterpret_cast<unsigned char *>(str));
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }

  SECTION("stores signed char* by copy") {
    char str[16];

    strcpy(str, "hello");
    variant.set(reinterpret_cast<signed char *>(str));
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("stores VLA by copy") {
    int n = 16;
    char str[n];

    strcpy(str, "hello");
    variant.set(str);
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }
#endif

  SECTION("stores std::string by copy") {
    std::string str;

    str = "hello";
    variant.set(str);
    str.replace(0, 5, "world");

    REQUIRE(variant == "hello");
  }

  SECTION("stores static JsonString by reference") {
    char str[16];

    strcpy(str, "hello");
    variant.set(JsonString(str, true));
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }

  SECTION("stores non-static JsonString by copy") {
    char str[16];

    strcpy(str, "hello");
    variant.set(JsonString(str, false));
    strcpy(str, "world");

    REQUIRE(variant == "hello");
  }
}

TEST_CASE("JsonVariant with not enough memory") {
  StaticJsonDocument<1> doc;

  JsonVariant v = doc.to<JsonVariant>();

  SECTION("std::string") {
    v.set(std::string("hello world!!"));
    REQUIRE(v.isNull());
  }

  SECTION("Serialized<std::string>") {
    v.set(serialized(std::string("hello world!!")));
    REQUIRE(v.isNull());
  }
}

TEST_CASE("JsonVariant::set(DynamicJsonDocument)") {
  DynamicJsonDocument doc1(1024);
  doc1["hello"] = "world";

  DynamicJsonDocument doc2(1024);
  JsonVariant v = doc2.to<JsonVariant>();

  // Should copy the doc
  v.set(doc1);
  doc1.clear();

  std::string json;
  serializeJson(doc2, json);
  REQUIRE(json == "{\"hello\":\"world\"}");
}
