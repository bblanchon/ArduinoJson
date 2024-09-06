// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonDocument::containsKey()") {
  JsonDocument doc;

  SECTION("returns true on object") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey("hello") == true);
  }

  SECTION("returns true when value is null") {
    doc["hello"] = static_cast<const char*>(0);

    REQUIRE(doc.containsKey("hello") == true);
  }

  SECTION("returns true when key is a std::string") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey("hello"_s) == true);
  }

  SECTION("returns false  on object") {
    doc["world"] = "hello";

    REQUIRE(doc.containsKey("hello") == false);
  }

  SECTION("returns false on array") {
    doc.add("hello");

    REQUIRE(doc.containsKey("hello") == false);
  }

  SECTION("returns false on null") {
    REQUIRE(doc.containsKey("hello") == false);
  }

  SECTION("support JsonVariant") {
    doc["hello"] = "world";
    doc["key"] = "hello";

    REQUIRE(doc.containsKey(doc["key"]) == true);
    REQUIRE(doc.containsKey(doc["foo"]) == false);
  }
}

TEST_CASE("MemberProxy::containsKey()") {
  JsonDocument doc;
  auto mp = doc["hello"];

  SECTION("containsKey(const char*)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key") == true);
    REQUIRE(mp.containsKey("key") == true);
  }

  SECTION("containsKey(std::string)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key"_s) == true);
    REQUIRE(mp.containsKey("key"_s) == true);
  }
}

TEST_CASE("JsonObject::containsKey()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["hello"] = 42;

  SECTION("returns true only if key is present") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("returns false after remove()") {
    obj.remove("hello");

    REQUIRE(false == obj.containsKey("hello"));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif

  SECTION("key is a JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(true == obj.containsKey(obj["key"]));
    REQUIRE(false == obj.containsKey(obj["hello"]));
  }

  SECTION("std::string") {
    REQUIRE(true == obj.containsKey("hello"_s));
  }

  SECTION("unsigned char[]") {
    unsigned char key[] = "hello";
    REQUIRE(true == obj.containsKey(key));
  }
}

TEST_CASE("JsonObjectConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = 42;
  auto obj = doc.as<JsonObjectConst>();

  SECTION("supports const char*") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("supports std::string") {
    REQUIRE(false == obj.containsKey("world"_s));
    REQUIRE(true == obj.containsKey("hello"_s));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif

  SECTION("supports JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(true == obj.containsKey(obj["key"]));
    REQUIRE(false == obj.containsKey(obj["hello"]));
  }
}

TEST_CASE("JsonVariant::containsKey()") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("returns false is unbound") {
    CHECK_FALSE(JsonVariant().containsKey("hello"));
  }

  SECTION("containsKey(const char*)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SECTION("containsKey(std::string)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

  SECTION("containsKey(JsonVariant)") {
    var["hello"] = "world";
    var["key"] = "hello";

    REQUIRE(var.containsKey(doc["key"]) == true);
    REQUIRE(var.containsKey(doc["foo"]) == false);
  }
}

TEST_CASE("JsonVariantConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = "world";
  JsonVariantConst var = doc.as<JsonVariant>();

  SECTION("support const char*") {
    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SECTION("support std::string") {
    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == var.containsKey(vla));
  }
#endif

  SECTION("support JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(var.containsKey(var["key"]) == true);
    REQUIRE(var.containsKey(var["foo"]) == false);
  }
}
