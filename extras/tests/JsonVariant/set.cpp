// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

enum ErrorCode { ERROR_01 = 1, ERROR_10 = 10 };

TEST_CASE("JsonVariant::set() when there is enough memory") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("const char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(static_cast<const char *>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "world");  // stores by pointer
  }

  SECTION("(const char*)0") {
    bool result = variant.set(static_cast<const char *>(0));

    REQUIRE(result == true);
    REQUIRE(variant.isNull());
  }

  SECTION("char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(str);
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }

  SECTION("(char*)0") {
    bool result = variant.set(static_cast<char *>(0));

    REQUIRE(result == true);
    REQUIRE(variant.isNull());
  }

  SECTION("unsigned char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(reinterpret_cast<unsigned char *>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }

  SECTION("signed char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(reinterpret_cast<signed char *>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("VLA") {
    size_t n = 16;
    char str[n];

    strcpy(str, "hello");
    bool result = variant.set(str);
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }
#endif

  SECTION("std::string") {
    std::string str;

    str = "hello";
    bool result = variant.set(str);
    str.replace(0, 5, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }

  SECTION("static JsonString") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(JsonString(str, JsonString::Linked));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "world");  // stores by pointer
  }

  SECTION("non-static JsonString") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(JsonString(str, JsonString::Copied));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
  }

  SECTION("enum") {
    ErrorCode code = ERROR_10;

    bool result = variant.set(code);

    REQUIRE(result == true);
    REQUIRE(variant.is<int>() == true);
    REQUIRE(variant.as<int>() == 10);
  }
}

TEST_CASE("JsonVariant::set() with not enough memory") {
  StaticJsonDocument<1> doc;

  JsonVariant v = doc.to<JsonVariant>();

  SECTION("std::string") {
    bool result = v.set(std::string("hello world!!"));

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("Serialized<std::string>") {
    bool result = v.set(serialized(std::string("hello world!!")));

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("char*") {
    char s[] = "hello world!!";
    bool result = v.set(s);

    REQUIRE(result == false);
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
