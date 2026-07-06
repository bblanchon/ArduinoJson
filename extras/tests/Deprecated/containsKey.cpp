// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("JsonDocument::containsKey()") {
  JsonDocument doc;

  SUBCASE("returns true on object") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey("hello") == true);
  }

  SUBCASE("returns true when value is null") {
    doc["hello"] = static_cast<const char*>(0);

    REQUIRE(doc.containsKey("hello") == true);
  }

  SUBCASE("returns true when key is a std::string") {
    doc["hello"] = "world";

    REQUIRE(doc.containsKey("hello"_s) == true);
  }

  SUBCASE("returns false  on object") {
    doc["world"] = "hello";

    REQUIRE(doc.containsKey("hello") == false);
  }

  SUBCASE("returns false on array") {
    doc.add("hello");

    REQUIRE(doc.containsKey("hello") == false);
  }

  SUBCASE("returns false on null") {
    REQUIRE(doc.containsKey("hello") == false);
  }

  SUBCASE("supports JsonVariant") {
    doc["hello"] = "world";
    doc["key"] = "hello";

    REQUIRE(doc.containsKey(doc["key"]) == true);
    REQUIRE(doc.containsKey(doc["foo"]) == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLAs") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    doc["hello"] = "world";

    REQUIRE(doc.containsKey(vla) == true);
  }
#endif
}

TEST_CASE("MemberProxy::containsKey()") {
  JsonDocument doc;
  const auto& mp = doc["hello"];

  SUBCASE("containsKey(const char*)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key") == true);
    REQUIRE(mp.containsKey("key") == true);
  }

  SUBCASE("containsKey(std::string)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key"_s) == true);
    REQUIRE(mp.containsKey("key"_s) == true);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLAs") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    mp["hello"] = "world";

    REQUIRE(mp.containsKey(vla) == true);
  }
#endif
}

TEST_CASE("JsonObject::containsKey()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["hello"] = 42;

  SUBCASE("returns true only if key is present") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SUBCASE("returns false after remove()") {
    obj.remove("hello");

    REQUIRE(false == obj.containsKey("hello"));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("key is a VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif

  SUBCASE("key is a JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(true == obj.containsKey(obj["key"]));
    REQUIRE(false == obj.containsKey(obj["hello"]));
  }

  SUBCASE("std::string") {
    REQUIRE(true == obj.containsKey("hello"_s));
  }

  SUBCASE("unsigned char[]") {
    unsigned char key[] = "hello";
    REQUIRE(true == obj.containsKey(key));
  }
}

TEST_CASE("JsonObjectConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = 42;
  auto obj = doc.as<JsonObjectConst>();

  SUBCASE("supports const char*") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SUBCASE("supports std::string") {
    REQUIRE(false == obj.containsKey("world"_s));
    REQUIRE(true == obj.containsKey("hello"_s));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif

  SUBCASE("supports JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(true == obj.containsKey(obj["key"]));
    REQUIRE(false == obj.containsKey(obj["hello"]));
  }
}

TEST_CASE("JsonVariant::containsKey()") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SUBCASE("returns false is unbound") {
    CHECK_FALSE(JsonVariant().containsKey("hello"));
  }

  SUBCASE("containsKey(const char*)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SUBCASE("containsKey(std::string)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

  SUBCASE("containsKey(JsonVariant)") {
    var["hello"] = "world";
    var["key"] = "hello";

    REQUIRE(var.containsKey(doc["key"]) == true);
    REQUIRE(var.containsKey(doc["foo"]) == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLAs") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    var["hello"] = "world";

    REQUIRE(var.containsKey(vla) == true);
  }
#endif
}

TEST_CASE("JsonVariantConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = "world";
  JsonVariantConst var = doc.as<JsonVariant>();

  SUBCASE("support const char*") {
    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SUBCASE("support std::string") {
    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == var.containsKey(vla));
  }
#endif

  SUBCASE("support JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(var.containsKey(var["key"]) == true);
    REQUIRE(var.containsKey(var["foo"]) == false);
  }
}
