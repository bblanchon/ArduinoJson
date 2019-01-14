// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonDocument") {
  DynamicJsonDocument doc(4096);

  SECTION("serializeJson()") {
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = "world";

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"hello\":\"world\"}");
  }

  SECTION("memoryUsage()") {
    SECTION("starts at zero") {
      REQUIRE(doc.memoryUsage() == 0);
    }

    SECTION("JSON_ARRAY_SIZE(0)") {
      doc.to<JsonArray>();
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
    }

    SECTION("JSON_ARRAY_SIZE(1)") {
      doc.to<JsonArray>().add(42);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1));
    }

    SECTION("JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(0)") {
      doc.to<JsonArray>().createNestedArray();
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(0));
    }
  }

  SECTION("capacity()") {
    SECTION("matches constructor argument") {
      DynamicJsonDocument doc2(256);
      REQUIRE(doc2.capacity() == 256);
    }

    SECTION("rounds up constructor argument") {
      DynamicJsonDocument doc2(253);
      REQUIRE(doc2.capacity() == 256);
    }
  }

  SECTION("memoryUsage()") {
    SECTION("Increases after adding value to array") {
      JsonArray arr = doc.to<JsonArray>();

      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
      arr.add(42);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1));
      arr.add(43);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
    }

    SECTION("Increases after adding value to object") {
      JsonObject obj = doc.to<JsonObject>();

      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      obj["a"] = 1;
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1));
      obj["b"] = 2;
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2));
    }
  }
}

TEST_CASE("DynamicJsonDocument copies") {
  SECTION("Copy constructor") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    doc1.nestingLimit = 42;

    DynamicJsonDocument doc2 = doc1;

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");

    REQUIRE(doc2.nestingLimit == 42);
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Copy assignment preserves the buffer when capacity is sufficient") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    doc1.nestingLimit = 42;

    DynamicJsonDocument doc2(doc1.capacity());
    doc2 = doc1;

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc2.nestingLimit == 42);
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Copy assignment realloc the buffer when capacity is insufficient") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    doc1.nestingLimit = 42;
    DynamicJsonDocument doc2(8);

    REQUIRE(doc2.capacity() < doc1.memoryUsage());
    doc2 = doc1;
    REQUIRE(doc2.capacity() >= doc1.memoryUsage());

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc2.nestingLimit == 42);
  }

  SECTION("Construct from StaticJsonDocument") {
    StaticJsonDocument<200> sdoc;
    deserializeJson(sdoc, "{\"hello\":\"world\"}");
    sdoc.nestingLimit = 42;

    DynamicJsonDocument ddoc = sdoc;

    std::string json;
    serializeJson(ddoc, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(ddoc.nestingLimit == 42);
    REQUIRE(ddoc.capacity() == sdoc.capacity());
  }

  SECTION("Assign from StaticJsonDocument") {
    DynamicJsonDocument ddoc(4096);
    ddoc.to<JsonVariant>().set(666);

    StaticJsonDocument<200> sdoc;
    deserializeJson(sdoc, "{\"hello\":\"world\"}");
    sdoc.nestingLimit = 42;

    ddoc = sdoc;

    std::string json;
    serializeJson(ddoc, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(ddoc.nestingLimit == 42);
  }
}
