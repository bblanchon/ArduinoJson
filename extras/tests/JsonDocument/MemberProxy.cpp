// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

typedef ArduinoJson::detail::MemberProxy<JsonDocument&, const char*>
    MemberProxy;

TEST_CASE("MemberProxy::add()") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("add(int)") {
    mp.add(42);

    REQUIRE(doc.as<std::string>() == "{\"hello\":[42]}");
  }

  SECTION("add(const char*)") {
    mp.add("world");

    REQUIRE(doc.as<std::string>() == "{\"hello\":[\"world\"]}");
  }
}

TEST_CASE("MemberProxy::clear()") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("size goes back to zero") {
    mp.add(42);
    mp.clear();

    REQUIRE(mp.size() == 0);
  }

  SECTION("isNull() return true") {
    mp.add("hello");
    mp.clear();

    REQUIRE(mp.isNull() == true);
  }
}

TEST_CASE("MemberProxy::operator==()") {
  JsonDocument doc;

  SECTION("1 vs 1") {
    doc["a"] = 1;
    doc["b"] = 1;

    REQUIRE(doc["a"] <= doc["b"]);
    REQUIRE(doc["a"] == doc["b"]);
    REQUIRE(doc["a"] >= doc["b"]);
    REQUIRE_FALSE(doc["a"] != doc["b"]);
    REQUIRE_FALSE(doc["a"] < doc["b"]);
    REQUIRE_FALSE(doc["a"] > doc["b"]);
  }

  SECTION("1 vs 2") {
    doc["a"] = 1;
    doc["b"] = 2;

    REQUIRE(doc["a"] != doc["b"]);
    REQUIRE(doc["a"] < doc["b"]);
    REQUIRE(doc["a"] <= doc["b"]);
    REQUIRE_FALSE(doc["a"] == doc["b"]);
    REQUIRE_FALSE(doc["a"] > doc["b"]);
    REQUIRE_FALSE(doc["a"] >= doc["b"]);
  }

  SECTION("'abc' vs 'bcd'") {
    doc["a"] = "abc";
    doc["b"] = "bcd";

    REQUIRE(doc["a"] != doc["b"]);
    REQUIRE(doc["a"] < doc["b"]);
    REQUIRE(doc["a"] <= doc["b"]);
    REQUIRE_FALSE(doc["a"] == doc["b"]);
    REQUIRE_FALSE(doc["a"] > doc["b"]);
    REQUIRE_FALSE(doc["a"] >= doc["b"]);
  }
}

TEST_CASE("MemberProxy::operator|()") {
  JsonDocument doc;

  SECTION("const char*") {
    doc["a"] = "hello";

    REQUIRE((doc["a"] | "world") == "hello"_s);
    REQUIRE((doc["b"] | "world") == "world"_s);
  }

  SECTION("Issue #1411") {
    doc["sensor"] = "gps";

    const char* test = "test";  // <- the literal must be captured in a variable
                                // to trigger the bug
    const char* sensor = doc["sensor"] | test;  // "gps"

    REQUIRE(sensor == "gps"_s);
  }

  SECTION("Issue #1415") {
    JsonObject object = doc.to<JsonObject>();
    object["hello"] = "world";

    JsonDocument emptyDoc;
    JsonObject anotherObject = object["hello"] | emptyDoc.to<JsonObject>();

    REQUIRE(anotherObject.isNull() == false);
    REQUIRE(anotherObject.size() == 0);
  }
}

TEST_CASE("MemberProxy::remove()") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("remove(int)") {
    mp.add(1);
    mp.add(2);
    mp.add(3);

    mp.remove(1);

    REQUIRE(mp.as<std::string>() == "[1,3]");
  }

  SECTION("remove(const char *)") {
    mp["a"] = 1;
    mp["b"] = 2;

    mp.remove("a");

    REQUIRE(mp.as<std::string>() == "{\"b\":2}");
  }

  SECTION("remove(std::string)") {
    mp["a"] = 1;
    mp["b"] = 2;

    mp.remove("b"_s);

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("remove(vla)") {
    mp["a"] = 1;
    mp["b"] = 2;

    size_t i = 4;
    char vla[i];
    strcpy(vla, "b");
    mp.remove(vla);

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }
#endif
}

TEST_CASE("MemberProxy::set()") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("set(int)") {
    mp.set(42);

    REQUIRE(doc.as<std::string>() == "{\"hello\":42}");
  }

  SECTION("set(const char*)") {
    mp.set("world");

    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("set(char[])") {  // issue #1191
    char s[] = "world";
    mp.set(s);
    strcpy(s, "!!!!!");

    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }
}

TEST_CASE("MemberProxy::size()") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("returns 0") {
    REQUIRE(mp.size() == 0);
  }

  SECTION("as an array, return 2") {
    mp.add(1);
    mp.add(2);

    REQUIRE(mp.size() == 2);
  }

  SECTION("as an object, return 2") {
    mp["a"] = 1;
    mp["b"] = 2;

    REQUIRE(mp.size() == 2);
  }
}

TEST_CASE("MemberProxy::operator[]") {
  JsonDocument doc;
  MemberProxy mp = doc["hello"];

  SECTION("set member") {
    mp["world"] = 42;

    REQUIRE(doc.as<std::string>() == "{\"hello\":{\"world\":42}}");
  }

  SECTION("set element") {
    mp[2] = 42;

    REQUIRE(doc.as<std::string>() == "{\"hello\":[null,null,42]}");
  }
}

TEST_CASE("MemberProxy cast to JsonVariantConst") {
  JsonDocument doc;
  doc["hello"] = "world";

  const MemberProxy mp = doc["hello"];

  JsonVariantConst var = mp;

  CHECK(var.as<std::string>() == "world");
}

TEST_CASE("MemberProxy cast to JsonVariant") {
  JsonDocument doc;
  doc["hello"] = "world";

  MemberProxy mp = doc["hello"];

  JsonVariant var = mp;

  CHECK(var.as<std::string>() == "world");

  var.set("toto");

  CHECK(doc.as<std::string>() == "{\"hello\":\"toto\"}");
}

TEST_CASE("Deduplicate keys") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("std::string") {
    doc[0]["example"_s] = 1;
    doc[1]["example"_s] = 2;

    const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
    const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
    CHECK(key1 == key2);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("char*") {
    char key[] = "example";
    doc[0][key] = 1;
    doc[1][key] = 2;

    const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
    const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
    CHECK(key1 == key2);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Arduino String") {
    doc[0][String("example")] = 1;
    doc[1][String("example")] = 2;

    const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
    const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
    CHECK(key1 == key2);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Flash string") {
    doc[0][F("example")] = 1;
    doc[1][F("example")] = 2;

    const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
    const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
    CHECK(key1 == key2);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }
}

TEST_CASE("MemberProxy under memory constraints") {
  TimebombAllocator timebomb(1);
  SpyingAllocator spy(&timebomb);
  JsonDocument doc(&spy);

  SECTION("key slot allocation fails") {
    timebomb.setCountdown(0);

    doc["hello"_s] = "world";

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.size() == 0);
    REQUIRE(doc.overflowed() == true);
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofPool()),
                         });
  }

  SECTION("value slot allocation fails") {
    timebomb.setCountdown(1);

    // fill the pool entirely, but leave one slot for the key
    doc["foo"][ARDUINOJSON_POOL_CAPACITY - 4] = 1;
    REQUIRE(doc.overflowed() == false);

    doc["hello"_s] = "world";

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.size() == 1);
    REQUIRE(doc.overflowed() == true);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             AllocateFail(sizeofPool()),
                         });
  }

  SECTION("key string allocation fails") {
    timebomb.setCountdown(1);

    doc["hello"_s] = "world";

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.size() == 0);
    REQUIRE(doc.overflowed() == true);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             AllocateFail(sizeofString("hello")),
                         });
  }
}
