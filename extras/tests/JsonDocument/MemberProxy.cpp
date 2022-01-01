// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemberProxy::add()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

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
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

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
  DynamicJsonDocument doc(4096);

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

TEST_CASE("MemberProxy::containsKey()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

  SECTION("containsKey(const char*)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey("key") == true);
    REQUIRE(mp.containsKey("key") == true);
  }

  SECTION("containsKey(std::string)") {
    mp["key"] = "value";

    REQUIRE(mp.containsKey(std::string("key")) == true);
    REQUIRE(mp.containsKey(std::string("key")) == true);
  }
}

TEST_CASE("MemberProxy::operator|()") {
  DynamicJsonDocument doc(4096);

  SECTION("const char*") {
    doc["a"] = "hello";

    REQUIRE((doc["a"] | "world") == std::string("hello"));
    REQUIRE((doc["b"] | "world") == std::string("world"));
  }

  SECTION("Issue #1411") {
    doc["sensor"] = "gps";

    const char *test = "test";  // <- the literal must be captured in a variable
                                // to trigger the bug
    const char *sensor = doc["sensor"] | test;  // "gps"

    REQUIRE(sensor == std::string("gps"));
  }

  SECTION("Issue #1415") {
    JsonObject object = doc.to<JsonObject>();
    object["hello"] = "world";

    StaticJsonDocument<0> emptyDoc;
    JsonObject anotherObject = object["hello"] | emptyDoc.to<JsonObject>();

    REQUIRE(anotherObject.isNull() == false);
    REQUIRE(anotherObject.size() == 0);
  }
}

TEST_CASE("MemberProxy::remove()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

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

    mp.remove(std::string("b"));

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("remove(vla)") {
    mp["a"] = 1;
    mp["b"] = 2;

    int i = 4;
    char vla[i];
    strcpy(vla, "b");
    mp.remove(vla);

    REQUIRE(mp.as<std::string>() == "{\"a\":1}");
  }
#endif
}

TEST_CASE("MemberProxy::set()") {
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

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
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

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
  DynamicJsonDocument doc(4096);
  MemberProxy<JsonDocument &, const char *> mp = doc["hello"];

  SECTION("set member") {
    mp["world"] = 42;

    REQUIRE(doc.as<std::string>() == "{\"hello\":{\"world\":42}}");
  }

  SECTION("set element") {
    mp[2] = 42;

    REQUIRE(doc.as<std::string>() == "{\"hello\":[null,null,42]}");
  }
}
