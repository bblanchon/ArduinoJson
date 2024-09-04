// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Literals.hpp"

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

enum MY_ENUM { ONE = 1, TWO = 2 };

TEST_CASE("JsonVariant::as()") {
  static const char* null = 0;

  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("not set") {
    REQUIRE(false == variant.as<bool>());
    REQUIRE(0 == variant.as<int>());
    REQUIRE(0.0f == variant.as<float>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE("null" == variant.as<std::string>());
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(float)") {
    variant.set(4.2f);

    REQUIRE(variant.as<bool>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE(variant.as<std::string>() == "4.2");
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<float>() == 4.2f);
    REQUIRE(variant.as<unsigned>() == 4U);
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(double)") {
    variant.set(4.2);

    REQUIRE(variant.as<bool>());
    REQUIRE(0 == variant.as<const char*>());
    REQUIRE(variant.as<std::string>() == "4.2");
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<double>() == 4.2);
    REQUIRE(variant.as<unsigned>() == 4U);
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(0.0)") {
    variant.set(0.0);

    REQUIRE(variant.as<bool>() == false);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(false)") {
    variant.set(false);

    REQUIRE(false == variant.as<bool>());
    REQUIRE(variant.as<double>() == 0.0);
    REQUIRE(variant.as<long>() == 0L);
    REQUIRE(variant.as<std::string>() == "false");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(true)") {
    variant.set(true);

    REQUIRE(variant.as<bool>());
    REQUIRE(variant.as<double>() == 1.0);
    REQUIRE(variant.as<long>() == 1L);
    REQUIRE(variant.as<std::string>() == "true");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(uint32_t)") {
    variant.set(4294967295U);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 4294967295.0);
    REQUIRE(variant.as<int32_t>() == 0);
    REQUIRE(variant.as<uint32_t>() == 4294967295U);
    REQUIRE(variant.as<uint64_t>() == 4294967295U);
    REQUIRE(variant.as<std::string>() == "4294967295");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(int32_t)") {
    variant.set(-2147483648LL);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == -2147483648LL);
    REQUIRE(variant.as<int32_t>() == -2147483648LL);
    REQUIRE(variant.as<int64_t>() == -2147483648LL);
    REQUIRE(variant.as<uint32_t>() == 0);
    REQUIRE(variant.as<uint64_t>() == 0);
    REQUIRE(variant.as<std::string>() == "-2147483648");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(uint64_t)") {
    variant.set(4294967296U);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == 4294967296.0);
    REQUIRE(variant.as<int32_t>() == 0);
    REQUIRE(variant.as<uint32_t>() == 0);
    REQUIRE(variant.as<uint64_t>() == 4294967296U);
    REQUIRE(variant.as<std::string>() == "4294967296");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("set(int64_t)") {
    variant.set(-2147483649LL);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<double>() == -2147483649LL);
    REQUIRE(variant.as<int32_t>() == 0);
    REQUIRE(variant.as<int64_t>() == -2147483649LL);
    REQUIRE(variant.as<uint32_t>() == 0);
    REQUIRE(variant.as<uint64_t>() == 0);
    REQUIRE(variant.as<std::string>() == "-2147483649");
    REQUIRE(variant.as<JsonString>().isNull());
    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
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
    REQUIRE(variant.as<const char*>() == "hello"_s);
    REQUIRE(variant.as<const char*>() == "hello"_s);
    REQUIRE(variant.as<std::string>() == "hello"_s);
    REQUIRE(variant.as<JsonString>() == "hello");
  }

  SECTION("set(std::string(\"4.2\"))") {
    variant.set("4.2"_s);

    REQUIRE(variant.as<bool>() == true);
    REQUIRE(variant.as<long>() == 4L);
    REQUIRE(variant.as<double>() == Approx(4.2));
    REQUIRE(variant.as<const char*>() == "4.2"_s);
    REQUIRE(variant.as<std::string>() == "4.2"_s);
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

  SECTION("set(serialized(\"hello\"))") {
    variant.set(serialized("hello"));

    REQUIRE(variant.as<MsgPackBinary>().data() == nullptr);
    REQUIRE(variant.as<MsgPackExtension>().data() == nullptr);
  }

  SECTION("to<JsonObject>()") {
    JsonObject obj = variant.to<JsonObject>();
    obj["key"] = "value";

    SECTION("as<bool>()") {
      REQUIRE(variant.as<bool>() == true);
    }

    SECTION("as<std::string>()") {
      REQUIRE(variant.as<std::string>() == "{\"key\":\"value\"}"_s);
    }

    SECTION("ObjectAsJsonObject") {
      JsonObject o = variant.as<JsonObject>();
      REQUIRE(o.size() == 1);
      REQUIRE(o["key"] == "value"_s);
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
      REQUIRE(variant.as<std::string>() == "[4,2]"_s);
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

  SECTION("Biggest int64 positive") {
    variant.set("9223372036854775807");
    REQUIRE(variant.as<long long>() == 9223372036854775807);
  }
#endif

  SECTION("as<enum>()") {
    variant.set(1);

    REQUIRE(variant.as<MY_ENUM>() == ONE);
  }
}
