// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonVariant::operator|()") {
  JsonDocument doc;
  JsonVariant variant = doc["value"].to<JsonVariant>();

  SUBCASE("null") {
    SUBCASE("null | const char*") {
      std::string result = variant | "default";
      REQUIRE(result == "default");
    }

    SUBCASE("null | int") {
      int result = variant | 42;
      REQUIRE(result == 42);
    }

    SUBCASE("null | bool") {
      bool result = variant | true;
      REQUIRE(result == true);
    }

    SUBCASE("null | ElementProxy") {
      doc["array"][0] = 42;

      JsonVariantConst result = variant | doc["array"][0];
      REQUIRE(result == 42);
    }

    SUBCASE("null | MemberProxy") {
      doc["other"] = 42;

      JsonVariantConst result = variant | doc["other"];
      REQUIRE(result == 42);
    }

    SUBCASE("ElementProxy | ElementProxy") {
      doc["array"][0] = 42;

      JsonVariantConst result = doc["array"][1] | doc["array"][0];
      REQUIRE(result == 42);
    }
  }

  SUBCASE("null") {
    variant.set(static_cast<const char*>(0));

    SUBCASE("null | const char*") {
      std::string result = variant | "default";
      REQUIRE(result == "default");
    }

    SUBCASE("null | int") {
      int result = variant | 42;
      REQUIRE(result == 42);
    }

    SUBCASE("null | bool") {
      bool result = variant | true;
      REQUIRE(result == true);
    }

    SUBCASE("null | ElementProxy") {
      doc["array"][0] = 42;

      JsonVariantConst result = variant | doc["array"][0];
      REQUIRE(result == 42);
    }

    SUBCASE("null | MemberProxy") {
      doc["other"] = 42;

      JsonVariantConst result = variant | doc["other"];
      REQUIRE(result == 42);
    }
  }

  SUBCASE("int | const char*") {
    variant.set(42);
    std::string result = variant | "default";
    REQUIRE(result == "default");
  }

  SUBCASE("int | uint8_t (out of range)") {
    variant.set(666);
    uint8_t result = variant | static_cast<uint8_t>(42);
    REQUIRE(result == 42);
  }

  SUBCASE("int | ElementProxy") {
    variant.set(42);
    doc["array"][0] = 666;
    JsonVariantConst result = variant | doc["array"][0];
    REQUIRE(result == 42);
  }

  SUBCASE("int | MemberProxy") {
    variant.set(42);
    doc["other"] = 666;
    JsonVariantConst result = variant | doc["other"];
    REQUIRE(result == 42);
  }

  SUBCASE("int | int") {
    variant.set(0);
    int result = variant | 666;
    REQUIRE(result == 0);
  }

  SUBCASE("double | int") {
    // NOTE: changed the behavior to fix #981
    variant.set(666.0);
    int result = variant | 42;
    REQUIRE(result == 42);
  }

  SUBCASE("bool | bool") {
    variant.set(false);
    bool result = variant | true;
    REQUIRE(result == false);
  }

  SUBCASE("int | bool") {
    variant.set(0);
    bool result = variant | true;
    REQUIRE(result == true);
  }

  SUBCASE("const char* | const char*") {
    variant.set("not default");
    std::string result = variant | "default";
    REQUIRE(result == "not default");
  }

  SUBCASE("const char* | char*") {
    char dflt[] = "default";
    variant.set("not default");
    std::string result = variant | dflt;
    REQUIRE(result == "not default");
  }

  SUBCASE("int | char*") {
    char dflt[] = "default";
    variant.set(42);
    std::string result = variant | dflt;
    REQUIRE(result == "default");
  }

  SUBCASE("const char* | int") {
    variant.set("not default");
    int result = variant | 42;
    REQUIRE(result == 42);
  }
}
