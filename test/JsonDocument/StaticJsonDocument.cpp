// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonDocument") {
  SECTION("capacity()") {
    SECTION("matches template argument") {
      StaticJsonDocument<256> doc;
      REQUIRE(doc.capacity() == 256);
    }

    SECTION("rounds up template argument") {
      StaticJsonDocument<253> doc;
      REQUIRE(doc.capacity() == 256);
    }
  }

  SECTION("serializeJson()") {
    StaticJsonDocument<200> doc;
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = "world";

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"hello\":\"world\"}");
  }

  SECTION("Copy assignment") {
    StaticJsonDocument<200> doc1, doc2;
    doc1.to<JsonVariant>().set(666);
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    doc1 = doc2;

    std::string json;
    serializeJson(doc1, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc1.nestingLimit == 42);
  }

  SECTION("Copy constructor") {
    StaticJsonDocument<200> doc1;
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    doc1.nestingLimit = 42;

    StaticJsonDocument<200> doc2 = doc1;

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc2.nestingLimit == 42);
  }

  SECTION("Assign from StaticJsonDocument of different capacity") {
    StaticJsonDocument<200> doc1;
    StaticJsonDocument<300> doc2;
    doc1.to<JsonVariant>().set(666);
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    doc1 = doc2;

    std::string json;
    serializeJson(doc1, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc1.nestingLimit == 42);
  }

  SECTION("Assign from DynamicJsonDocument") {
    StaticJsonDocument<200> doc1;
    DynamicJsonDocument doc2(4096);
    doc1.to<JsonVariant>().set(666);
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    doc1 = doc2;

    std::string json;
    serializeJson(doc1, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc1.nestingLimit == 42);
  }

  SECTION("Construct from StaticJsonDocument of different size") {
    StaticJsonDocument<300> doc2;
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    StaticJsonDocument<200> doc1 = doc2;

    std::string json;
    serializeJson(doc1, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc1.nestingLimit == 42);
  }

  SECTION("Construct from DynamicJsonDocument") {
    DynamicJsonDocument doc2(4096);
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    StaticJsonDocument<200> doc1 = doc2;

    std::string json;
    serializeJson(doc1, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc1.nestingLimit == 42);
  }
}
