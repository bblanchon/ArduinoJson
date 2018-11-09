// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonDocument") {
  DynamicJsonDocument doc;

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

  SECTION("Copy constructor") {
    deserializeJson(doc, "{\"hello\":\"world\"}");
    doc.nestingLimit = 42;

    DynamicJsonDocument doc2 = doc;

    std::string json;
    serializeJson(doc2, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc2.nestingLimit == 42);
  }

  SECTION("Copy assignment") {
    DynamicJsonDocument doc2;
    deserializeJson(doc2, "{\"hello\":\"world\"}");
    doc2.nestingLimit = 42;

    doc = doc2;

    std::string json;
    serializeJson(doc, json);
    REQUIRE(json == "{\"hello\":\"world\"}");
    REQUIRE(doc.nestingLimit == 42);
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
  }

  SECTION("Assign from StaticJsonDocument") {
    DynamicJsonDocument ddoc;
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

  SECTION("memoryUsage()") {
    SECTION("Increases after adding value to array") {
      JsonArray arr = doc.to<JsonArray>();

      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
      arr.add(42);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1));
      arr.add(43);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
    }

    SECTION("Decreases after removing value from array") {
      JsonArray arr = doc.to<JsonArray>();
      arr.add(42);
      arr.add(43);

      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
      arr.remove(1);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1));
      arr.remove(0);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
    }

    SECTION("Increases after adding value to object") {
      JsonObject obj = doc.to<JsonObject>();

      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
      obj["a"] = 1;
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1));
      obj["b"] = 2;
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2));
    }

    SECTION("Decreases after removing value from object") {
      JsonObject obj = doc.to<JsonObject>();
      obj["a"] = 1;
      obj["b"] = 2;

      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(2));
      obj.remove("a");
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1));
      obj.remove("b");
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
    }

    SECTION("Decreases after removing nested object from array") {
      JsonArray arr = doc.to<JsonArray>();
      JsonObject obj = arr.createNestedObject();
      obj["hello"] = "world";

      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1));
      arr.remove(0);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
    }

    SECTION("Decreases after removing nested array from object") {
      JsonObject obj = doc.to<JsonObject>();
      JsonArray arr = obj.createNestedArray("hello");
      arr.add("world");

      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1));
      obj.remove("hello");
      REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
    }
  }
}
