// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_USE_LONG_LONG 0
#define ARDUINOJSON_ENABLE_NAN 1
#define ARDUINOJSON_ENABLE_INFINITY 1

#include <ArduinoJson.h>
#include <limits.h>
#include <catch.hpp>

namespace my {
using ArduinoJson::detail::isinf;
using ArduinoJson::detail::isnan;
}  // namespace my

TEST_CASE("deserializeJson() with a top-level number") {
  JsonDocument doc;

  SECTION("Integer") {
    SECTION("0") {
      auto err = deserializeJson(doc, "0");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<int>() == true);
      REQUIRE(doc.as<int>() == 0);
      REQUIRE(doc.as<std::string>() == "0");  // issue #808
    }

    SECTION("Negative") {
      auto err = deserializeJson(doc, "-42");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<int>());
      REQUIRE_FALSE(doc.is<bool>());
      REQUIRE(doc.as<int>() == -42);
    }

#if LONG_MAX == 2147483647
    SECTION("LONG_MAX") {
      auto err = deserializeJson(doc, "2147483647");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<long>() == true);
      REQUIRE(doc.as<long>() == LONG_MAX);
    }

    SECTION("LONG_MAX + 1") {
      auto err = deserializeJson(doc, "2147483648");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<long>() == false);
      REQUIRE(doc.is<float>() == true);
    }
#endif

#if LONG_MIN == -2147483648
    SECTION("LONG_MIN") {
      auto err = deserializeJson(doc, "-2147483648");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<long>() == true);
      REQUIRE(doc.as<long>() == LONG_MIN);
    }

    SECTION("LONG_MIN - 1") {
      auto err = deserializeJson(doc, "-2147483649");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<long>() == false);
      REQUIRE(doc.is<float>() == true);
    }
#endif

#if ULONG_MAX == 4294967295
    SECTION("ULONG_MAX") {
      auto err = deserializeJson(doc, "4294967295");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<unsigned long>() == true);
      REQUIRE(doc.as<unsigned long>() == ULONG_MAX);
      REQUIRE(doc.is<long>() == false);
    }

    SECTION("ULONG_MAX + 1") {
      auto err = deserializeJson(doc, "4294967296");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<unsigned long>() == false);
      REQUIRE(doc.is<float>() == true);
    }
#endif
  }

  SECTION("Floats") {
    SECTION("Double") {
      auto err = deserializeJson(doc, "-1.23e+4");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE_FALSE(doc.is<int>());
      REQUIRE(doc.is<double>());
      REQUIRE(doc.as<double>() == Approx(-1.23e+4));
    }

    SECTION("NaN") {
      auto err = deserializeJson(doc, "NaN");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<float>() == true);
      REQUIRE(my::isnan(doc.as<float>()));
    }

    SECTION("Infinity") {
      auto err = deserializeJson(doc, "Infinity");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<float>() == true);
      REQUIRE(my::isinf(doc.as<float>()));
    }

    SECTION("+Infinity") {
      auto err = deserializeJson(doc, "+Infinity");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<float>() == true);
      REQUIRE(my::isinf(doc.as<float>()));
    }

    SECTION("-Infinity") {
      auto err = deserializeJson(doc, "-Infinity");
      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.is<float>() == true);
      REQUIRE(my::isinf(doc.as<float>()));
    }
  }
}
