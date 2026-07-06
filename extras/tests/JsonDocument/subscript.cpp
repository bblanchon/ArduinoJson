// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::operator[]") {
  JsonDocument doc;
  const JsonDocument& cdoc = doc;

  SECTION("object") {
    doc["abc"_s] = "ABC";
    doc["abcd"_s] = "ABCD";

    SECTION("const char*") {
      const char* key = "abc";
      REQUIRE(doc[key] == "ABC");
      REQUIRE(cdoc[key] == "ABC");
    }

    SECTION("string literal") {
      REQUIRE(doc["abc"] == "ABC");
      REQUIRE(cdoc["abc"] == "ABC");
      REQUIRE(doc["abcd"] == "ABCD");
      REQUIRE(cdoc["abcd"] == "ABCD");
    }

    SECTION("std::string") {
      REQUIRE(doc["abc"_s] == "ABC");
      REQUIRE(cdoc["abc"_s] == "ABC");
      REQUIRE(doc["abcd"_s] == "ABCD");
      REQUIRE(cdoc["abcd"_s] == "ABCD");
    }

    SECTION("JsonVariant") {
      doc["key1"] = "abc";
      doc["key2"] = "abcd"_s;
      doc["key3"] = "foo";

      CHECK(doc[doc["key1"]] == "ABC");
      CHECK(doc[doc["key2"]] == "ABCD");
      CHECK(doc[doc["key3"]] == nullptr);
      CHECK(doc[doc["key4"]] == nullptr);

      CHECK(cdoc[cdoc["key1"]] == "ABC");
      CHECK(cdoc[cdoc["key2"]] == "ABCD");
      CHECK(cdoc[cdoc["key3"]] == nullptr);
      CHECK(cdoc[cdoc["key4"]] == nullptr);
    }

    SECTION("supports operator|") {
      REQUIRE((doc["abc"] | "nope") == "ABC"_s);
      REQUIRE((doc["def"] | "nope") == "nope"_s);
    }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
    SECTION("supports VLAs") {
      size_t i = 16;
      char vla[i];
      strcpy(vla, "hello");

      doc[vla] = "world";

      REQUIRE(doc[vla] == "world");
      REQUIRE(cdoc[vla] == "world");
    }
#endif
  }

  SECTION("array") {
    deserializeJson(doc, "[\"hello\",\"world\"]");

    SECTION("int") {
      REQUIRE(doc[1] == "world");
      REQUIRE(cdoc[1] == "world");
    }

    SECTION("JsonVariant") {
      doc[2] = 1;
      REQUIRE(doc[doc[2]] == "world");
      REQUIRE(cdoc[doc[2]] == "world");
    }
  }
}

TEST_CASE("JsonDocument automatically promotes to object") {
  JsonDocument doc;

  doc["one"]["two"]["three"] = 4;

  REQUIRE(doc["one"]["two"]["three"] == 4);
}

TEST_CASE("JsonDocument automatically promotes to array") {
  JsonDocument doc;

  doc[2] = 2;

  REQUIRE(doc.as<std::string>() == "[null,null,2]");
}

TEST_CASE("JsonDocument::operator[] key storage") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("string literal") {
    doc["hello"] = 0;

    REQUIRE(doc.as<std::string>() == "{\"hello\":0}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("const char*") {
    const char* key = "hello";
    doc[key] = 0;

    REQUIRE(doc.as<std::string>() == "{\"hello\":0}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("char[]") {
    char key[] = "hello";
    doc[key] = 0;

    REQUIRE(doc.as<std::string>() == "{\"hello\":0}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SECTION("std::string") {
    doc["hello"_s] = 0;

    REQUIRE(doc.as<std::string>() == "{\"hello\":0}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }
#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    doc[vla] = 0;

    REQUIRE(doc.as<std::string>() == "{\"hello\":0}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }
#endif
}
