// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant and strings") {
  DynamicJsonDocument doc;
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
}

TEST_CASE("JsonVariant::set() should release string memory") {
  DynamicJsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  variant.set(std::string("hello"));
  REQUIRE(doc.memoryUsage() == JSON_STRING_SIZE(6));

  SECTION("int") {
    variant.set(-42);
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("unsigned int") {
    variant.set(42U);
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("bool") {
    variant.set(true);
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("float") {
    variant.set(3.14);
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("const char*") {
    variant.set("hello");
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("std::string") {
    variant.set(std::string("X"));
    REQUIRE(doc.memoryUsage() == JSON_STRING_SIZE(2));
  }

  SECTION("SerializedValue<const char*>") {
    variant.set(serialized("[42]"));
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("SerializedValue<std::string>") {
    variant.set(serialized(std::string("42")));
    REQUIRE(doc.memoryUsage() == JSON_STRING_SIZE(2));
  }

  SECTION("StringInMemoryPool") {
    DeserializationError err =
        deserializeJson(doc, std::string("{\"A\":\"hello\",\"A\":\"B\"}"));
    REQUIRE(err == DeserializationError::Ok);
    // it stores the key twice, but should release "hello"
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + 3 * JSON_STRING_SIZE(2));
  }
}

TEST_CASE("JsonVariant with not enough memory") {
  StaticJsonDocument<1> doc;

  JsonVariant v = doc.to<JsonVariant>();

  SECTION("std::string") {
    v.set(std::string("hello"));
    REQUIRE(v.isNull());
  }

  SECTION("Serialized<std::string>") {
    v.set(serialized(std::string("hello")));
    REQUIRE(v.isNull());
  }
}
