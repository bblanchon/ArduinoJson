// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

enum MY_ENUM { ONE = 1, TWO = 2 };

TEST_CASE("JsonVariant::as()") {
  static const char* null = 0;

  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("not set") {
    REQUIRE(false == variant.as<bool>());
    REQUIRE(0 == variant.as<int>());
    REQUIRE(0.0f == variant.as<float>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE("null" == variant.as<std::string>());
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(4.2)") {
    variant.set(4.2);

    REQUIRE(variant.as<bool>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE(variant.as<std::string>() == "4.2");
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<unsigned>() == 4U);
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(0.0)") {
    variant.set(0.0);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(false)") {
    variant.set(false);

    REQUIRE(false == variant.as<bool>());
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<std::string>() == "false");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(true)") {
    variant.set(true);

    REQUIRE(variant.as<bool>());
    REQUIRE(variant.as<double>() == 1.0);
    REQUIRE(variant.as<long>() == 1L);
    REQUIRE(variant.as<std::string>() == "true");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(42)") {
    variant.set(42);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 42.0);
    REQUIRE(variant.as<int>() == 42);
    REQUIRE(variant.as<unsigned int>() == 42U);  // issue #1601
    REQUIRE(variant.as<std::string>() == "42");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(42L)") {
    variant.set(42L);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 42.0);
    REQUIRE(variant.as<std::string>() == "42");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(-42L)") {
    variant.set(-42L);

    REQUIRE(variant.as<double>() == -42.0);
    REQUIRE(variant.as<std::string>() == "-42");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(42UL)") {
    variant.set(42UL);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 42.0);
    REQUIRE(variant.as<std::string>() == "42");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(0L)") {
    variant.set(0L);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<std::string>() == "0");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(0UL)") {
    variant.set(0UL);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<std::string>() == "0");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(null)") {
    variant.set(null);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<std::string>() == "null");
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(\"42\")") {
    variant.set("42");

    REQUIRE(variant.as<long>() == 42L);
    REQUIRE(variant.as<JsonString>() == "42");
    REQUIRE(variant.as<JsonString>().isLinked() == true);
  }

  SECTION("set(\"hello\")") {
    variant.set("hello");

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<const char*>() == std::string("hello"));
    REQUIRE(variant.as<const char*>() == std::string("hello"));
    REQUIRE(variant.as<std::string>() == std::string("hello"));
    REQUIRE(variant.as<JsonString>() == "hello");
  }

  SECTION("set(std::string(\"4.2\"))") {
    variant.set(std::string("4.2"));

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<double>() == 4.2);
    REQUIRE(variant.as<const char*>() == std::string("4.2"));
    REQUIRE(variant.as<std::string>() == std::string("4.2"));
    REQUIRE(variant.as<JsonString>() == "4.2");
    REQUIRE(variant.as<JsonString>().isLinked() == false);
  }

  SECTION("set(\"true\")") {
    variant.set("true");

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<int>() == 0);
    REQUIRE(variant.as<JsonString>() == "true");
  }

  SECTION("set(-1e300)") {
    variant.set(-1e300);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == -1e300);
    REQUIRE(variant.as<float>() < 0);
    REQUIRE(my::isinf(variant.as<float>()));
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(1e300)") {
    variant.set(1e300);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 1e300);
    REQUIRE(variant.as<float>() > 0);
    REQUIRE(my::isinf(variant.as<float>()));
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("set(1e-300)") {
    variant.set(1e-300);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 1e-300);
    REQUIRE(variant.as<float>() == 0);
    REQUIRE(variant.as<JsonString>().isNull());
  }

  SECTION("to<JsonObject>()") {
    JsonObject obj = variant.to<JsonObject>();
    obj["key"] = "value";

    SECTION("as<bool>()") {
      REQUIRE(variant.as<bool>() == true);
    }

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

    SECTION("as<bool>()") {
      REQUIRE(variant.as<bool>() == true);
    }

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

    REQUIRE(cvar.as<bool>() == true);
    REQUIRE(cvar.as<long>() == 0L);
    REQUIRE(cvar.as<const char*>() == std::string("hello"));
    REQUIRE(cvar.as<std::string>() == std::string("hello"));
  }

  SECTION("as<enum>()") {
    variant.set(1);

    REQUIRE(variant.as<MY_ENUM>() == ONE);
  }
}
