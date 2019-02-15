// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::as()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("not set") {
    REQUIRE(false == variant.as<bool>());
    REQUIRE(0 == variant.as<int>());
    REQUIRE(0.0f == variant.as<float>());
    REQUIRE(0 == variant.as<char*>());
    REQUIRE("null" == variant.as<std::string>());
  }

  SECTION("set(4.2)") {
    variant.set(4.2);

    REQUIRE(variant.as<bool>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE(variant.as<std::string>() == "4.2");
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<unsigned>() == 4U);
  }

  SECTION("set(0.0)") {
    variant.set(0.0);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<long>() == 0L);
  }

  SECTION("set(false)") {
    variant.set(false);

    REQUIRE(false == variant.as<bool>());
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<std::string>() == "false");
  }

  SECTION("set(true)") {
    variant.set(true);

    REQUIRE(variant.as<bool>());
    REQUIRE(variant.as<double>() == 1.0);
    REQUIRE(variant.as<long>() == 1L);
    REQUIRE(variant.as<std::string>() == "true");
  }

  SECTION("set(42L)") {
    variant.set(42L);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 42.0);
    REQUIRE(variant.as<std::string>() == "42");
  }

  SECTION("set(-42L)") {
    variant.set(-42L);

    REQUIRE(variant.as<double>() == -42.0);
    REQUIRE(variant.as<std::string>() == "-42");
  }

  SECTION("set(0L)") {
    variant.set(0L);

    SECTION("as<bool>()") {
      REQUIRE(false == variant.as<bool>());
    }

    SECTION("as<double>()") {
      REQUIRE(variant.as<double>() == 0.0);
    }
  }

  SECTION("set(null)") {
    variant.set(null);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<std::string>() == "null");
  }

  SECTION("set(\"42\")") {
    variant.set("42");

    REQUIRE(variant.as<bool>());
    REQUIRE(variant.as<long>() == 42L);
  }

  SECTION("set(\"hello\")") {
    variant.set("hello");

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<const char*>() == std::string("hello"));
    REQUIRE(variant.as<char*>() == std::string("hello"));
    REQUIRE(variant.as<std::string>() == std::string("hello"));
  }

  SECTION("set(std::string(\"4.2\"))") {
    variant.set(std::string("4.2"));

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<double>() == 4.2);
    REQUIRE(variant.as<char*>() == std::string("4.2"));
    REQUIRE(variant.as<std::string>() == std::string("4.2"));
  }

  SECTION("set(\"true\")") {
    variant.set("true");

    REQUIRE(variant.as<bool>());
    REQUIRE(variant.as<long>() == 1L);
  }

  SECTION("to<JsonObject>()") {
    JsonObject obj = variant.to<JsonObject>();
    obj["key"] = "value";

    SECTION("as<std::string>()") {
      REQUIRE(variant.as<std::string>() == std::string("{\"key\":\"value\"}"));
    }

    SECTION("ObjectAsJsonObject") {
      JsonObject o = variant.as<JsonObject>();
      REQUIRE(o.size() == 1);
      REQUIRE(o["key"] == std::string("value"));
    }
  }

  SECTION("to<JsonArray>()") {
    JsonArray arr = variant.to<JsonArray>();
    arr.add(4);
    arr.add(2);

    SECTION("as<std::string>()") {
      REQUIRE(variant.as<std::string>() == std::string("[4,2]"));
    }

    SECTION("as<JsonArray>()") {
      JsonArray a = variant.as<JsonArray>();
      REQUIRE(a.size() == 2);
      REQUIRE(a[0] == 4);
      REQUIRE(a[1] == 2);
    }
  }

#if ARDUINOJSON_USE_LONG_LONG
  SECTION("Smallest int64 negative") {
    variant.set("-9223372036854775808");
    REQUIRE(variant.as<long long>() == -9223372036854775807 - 1);
  }

  SECTION("Biggerst int64 positive") {
    variant.set("9223372036854775807");
    REQUIRE(variant.as<long long>() == 9223372036854775807);
  }
#endif

  SECTION("should work on JsonVariantConst") {
    variant.set("hello");

    JsonVariantConst cvar = variant;

    REQUIRE(cvar.as<bool>() == false);
    REQUIRE(cvar.as<long>() == 0L);
    REQUIRE(cvar.as<const char*>() == std::string("hello"));
    REQUIRE(cvar.as<char*>() == std::string("hello"));
    // REQUIRE(cvar.as<std::string>() == std::string("hello"));
  }
}
