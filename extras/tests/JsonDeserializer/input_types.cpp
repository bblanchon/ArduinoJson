// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

#include <catch.hpp>
#include <sstream>

#include "Allocators.hpp"
#include "CustomReader.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofObject;

TEST_CASE("deserializeJson(char*)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  char input[] = "{\"hello\":\"world\"}";

  DeserializationError err = deserializeJson(doc, input);

  REQUIRE(err == DeserializationError::Ok);

  REQUIRE(spy.log() ==
          AllocatorLog{
              Allocate(sizeofStringBuffer()),
              Reallocate(sizeofStringBuffer(), sizeofString("hello")),
              Allocate(sizeofPool()),
              Allocate(sizeofStringBuffer()),
              Reallocate(sizeofStringBuffer(), sizeofString("world")),
              Reallocate(sizeofPool(), sizeofObject(1)),
          });
}

TEST_CASE("deserializeJson(unsigned char*, unsigned int)") {  // issue #1897
  JsonDocument doc;

  unsigned char input[] = "{\"hello\":\"world\"}";
  unsigned char* input_ptr = input;
  unsigned int size = sizeof(input);

  DeserializationError err = deserializeJson(doc, input_ptr, size);

  REQUIRE(err == DeserializationError::Ok);
}

TEST_CASE("deserializeJson(uint8_t*, size_t)") {  // issue #1898
  JsonDocument doc;

  uint8_t input[] = "{\"hello\":\"world\"}";
  uint8_t* input_ptr = input;
  size_t size = sizeof(input);

  DeserializationError err = deserializeJson(doc, input_ptr, size);

  REQUIRE(err == DeserializationError::Ok);
}

TEST_CASE("deserializeJson(const std::string&)") {
  JsonDocument doc;

  SECTION("should accept const string") {
    const std::string input("[42]");

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should accept temporary string") {
    DeserializationError err = deserializeJson(doc, "[42]"_s);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should duplicate content") {
    std::string input("[\"hello\"]");

    DeserializationError err = deserializeJson(doc, input);
    input[2] = 'X';  // alter the string tomake sure we made a copy

    JsonArray array = doc.as<JsonArray>();
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE("hello"_s == array[0]);
  }
}

TEST_CASE("deserializeJson(std::istream&)") {
  JsonDocument doc;

  SECTION("array") {
    std::istringstream json(" [ 42 ] ");

    DeserializationError err = deserializeJson(doc, json);
    JsonArray arr = doc.as<JsonArray>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(1 == arr.size());
    REQUIRE(42 == arr[0]);
  }

  SECTION("object") {
    std::istringstream json(" { hello : 'world' }");

    DeserializationError err = deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(1 == obj.size());
    REQUIRE("world"_s == obj["hello"]);
  }

  SECTION("Should not read after the closing brace of an empty object") {
    std::istringstream json("{}123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing brace") {
    std::istringstream json("{\"hello\":\"world\"}123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing bracket of an empty array") {
    std::istringstream json("[]123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing bracket") {
    std::istringstream json("[\"hello\",\"world\"]123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing quote") {
    std::istringstream json("\"hello\"123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }
}

#ifdef HAS_VARIABLE_LENGTH_ARRAY
TEST_CASE("deserializeJson(VLA)") {
  size_t i = 9;
  char vla[i];
  strcpy(vla, "{\"a\":42}");

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, vla);

  REQUIRE(err == DeserializationError::Ok);
}
#endif

TEST_CASE("deserializeJson(CustomReader)") {
  JsonDocument doc;
  CustomReader reader("[4,2]");
  DeserializationError err = deserializeJson(doc, reader);

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc.size() == 2);
  REQUIRE(doc[0] == 4);
  REQUIRE(doc[1] == 2);
}

TEST_CASE("deserializeJson(JsonDocument&, MemberProxy)") {
  JsonDocument doc1;
  doc1["payload"] = "[4,2]";

  JsonDocument doc2;
  DeserializationError err = deserializeJson(doc2, doc1["payload"]);

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc2.size() == 2);
  REQUIRE(doc2[0] == 4);
  REQUIRE(doc2[1] == 2);
}

TEST_CASE("deserializeJson(JsonDocument&, JsonVariant)") {
  JsonDocument doc1;
  doc1["payload"] = "[4,2]";

  JsonDocument doc2;
  DeserializationError err =
      deserializeJson(doc2, doc1["payload"].as<JsonVariant>());

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc2.size() == 2);
  REQUIRE(doc2[0] == 4);
  REQUIRE(doc2[1] == 2);
}

TEST_CASE("deserializeJson(JsonDocument&, JsonVariantConst)") {
  JsonDocument doc1;
  doc1["payload"] = "[4,2]";

  JsonDocument doc2;
  DeserializationError err =
      deserializeJson(doc2, doc1["payload"].as<JsonVariantConst>());

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc2.size() == 2);
  REQUIRE(doc2[0] == 4);
  REQUIRE(doc2[1] == 2);
}

TEST_CASE("deserializeJson(JsonDocument&, ElementProxy)") {
  JsonDocument doc1;
  doc1[0] = "[4,2]";

  JsonDocument doc2;
  DeserializationError err = deserializeJson(doc2, doc1[0]);

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc2.size() == 2);
  REQUIRE(doc2[0] == 4);
  REQUIRE(doc2[1] == 2);
}
