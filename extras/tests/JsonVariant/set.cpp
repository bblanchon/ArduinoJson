// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofObject;

enum ErrorCode { ERROR_01 = 1, ERROR_10 = 10 };

TEST_CASE("JsonVariant::set() when there is enough memory") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("const char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(static_cast<const char*>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "world");  // stores by pointer
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("(const char*)0") {
    bool result = variant.set(static_cast<const char*>(0));

    REQUIRE(result == true);
    REQUIRE(variant.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(str);
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("(char*)0") {
    bool result = variant.set(static_cast<char*>(0));

    REQUIRE(result == true);
    REQUIRE(variant.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("unsigned char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(reinterpret_cast<unsigned char*>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("signed char*") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(reinterpret_cast<signed char*>(str));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
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
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
  }
#endif

  SECTION("std::string") {
    std::string str;

    str = "hello";
    bool result = variant.set(str);
    str.replace(0, 5, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("static JsonString") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(JsonString(str, JsonString::Linked));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "world");  // stores by pointer
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("non-static JsonString") {
    char str[16];

    strcpy(str, "hello");
    bool result = variant.set(JsonString(str, JsonString::Copied));
    strcpy(str, "world");

    REQUIRE(result == true);
    REQUIRE(variant == "hello");  // stores by copy
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("enum") {
    ErrorCode code = ERROR_10;

    bool result = variant.set(code);

    REQUIRE(result == true);
    REQUIRE(variant.is<int>() == true);
    REQUIRE(variant.as<int>() == 10);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("float") {
    bool result = variant.set(1.2f);

    REQUIRE(result == true);
    REQUIRE(variant.is<float>() == true);
    REQUIRE(variant.as<float>() == 1.2f);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("double") {
    bool result = variant.set(1.2);
    doc.shrinkToFit();

    REQUIRE(result == true);
    REQUIRE(variant.is<double>() == true);
    REQUIRE(variant.as<double>() == 1.2);
    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeofPool(1)),  // one extension slot
            });
  }

  SECTION("int32_t") {
    bool result = variant.set(int32_t(42));

    REQUIRE(result == true);
    REQUIRE(variant.is<int32_t>() == true);
    REQUIRE(variant.as<int32_t>() == 42);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("int64_t") {
    bool result = variant.set(int64_t(-2147483649LL));
    doc.shrinkToFit();

    REQUIRE(result == true);
    REQUIRE(variant.is<int64_t>() == true);
    REQUIRE(variant.as<int64_t>() == -2147483649LL);
    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeofPool(1)),  // one extension slot
            });
  }

  SECTION("uint32_t") {
    bool result = variant.set(uint32_t(42));

    REQUIRE(result == true);
    REQUIRE(variant.is<uint32_t>() == true);
    REQUIRE(variant.as<uint32_t>() == 42);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("uint64_t") {
    bool result = variant.set(uint64_t(4294967296));
    doc.shrinkToFit();

    REQUIRE(result == true);
    REQUIRE(variant.is<uint64_t>() == true);
    REQUIRE(variant.as<uint64_t>() == 4294967296);
    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeofPool(1)),  // one extension slot
            });
  }

  SECTION("JsonDocument") {
    JsonDocument doc1;
    doc1["hello"] = "world";

    // Should copy the doc
    variant.set(doc1);
    doc1.clear();

    std::string json;
    serializeJson(doc, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
  }
}

TEST_CASE("JsonVariant::set() with not enough memory") {
  JsonDocument doc(FailingAllocator::instance());

  JsonVariant v = doc.to<JsonVariant>();

  SECTION("std::string") {
    bool result = v.set("hello world!!"_s);

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("Serialized<std::string>") {
    bool result = v.set(serialized("hello world!!"_s));

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("char*") {
    char s[] = "hello world!!";
    bool result = v.set(s);

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("float") {
    bool result = v.set(1.2f);

    REQUIRE(result == true);
    REQUIRE(v.is<float>());
  }

  SECTION("double") {
    bool result = v.set(1.2);

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("int32_t") {
    bool result = v.set(-42);

    REQUIRE(result == true);
    REQUIRE(v.is<int32_t>());
  }

  SECTION("int64_t") {
    bool result = v.set(-2147483649LL);

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }

  SECTION("uint32_t") {
    bool result = v.set(42);

    REQUIRE(result == true);
    REQUIRE(v.is<uint32_t>());
  }

  SECTION("uint64_t") {
    bool result = v.set(4294967296U);

    REQUIRE(result == false);
    REQUIRE(v.isNull());
  }
}

TEST_CASE("JsonVariant::set() releases the previous value") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc["hello"] = "world"_s;
  spy.clearLog();

  JsonVariant v = doc["hello"];

  SECTION("int") {
    v.set(42);
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("bool") {
    v.set(false);
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("const char*") {
    v.set("hello");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("float") {
    v.set(1.2);
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("Serialized<const char*>") {
    v.set(serialized("[]"));
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                             Allocate(sizeofString("[]")),
                         });
  }
}

TEST_CASE("JsonVariant::set() reuses extension slot") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant variant = doc.to<JsonVariant>();

  variant.set(1.2);
  doc.shrinkToFit();
  spy.clearLog();

  SECTION("double") {
    bool result = variant.set(3.4);

    REQUIRE(result == true);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("int64_t") {
    bool result = variant.set(-2147483649LL);

    REQUIRE(result == true);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("uint64_t") {
    bool result = variant.set(4294967296U);

    REQUIRE(result == true);
    REQUIRE(spy.log() == AllocatorLog{});
  }
}
