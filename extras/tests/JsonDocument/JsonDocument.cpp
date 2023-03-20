// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::addPadding;

static void REQUIRE_JSON(JsonDocument& doc, const std::string& expected) {
  std::string json;
  serializeJson(doc, json);
  REQUIRE(json == expected);
}

TEST_CASE("JsonDocument") {
  JsonDocument doc(4096);

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
      JsonDocument doc2(256);
      REQUIRE(doc2.capacity() == 256);
    }

    SECTION("rounds up constructor argument") {
      JsonDocument doc2(253);
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

TEST_CASE("JsonDocument constructor") {
  SECTION("Copy constructor") {
    JsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    JsonDocument doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");

    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Construct from JsonObject") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2 = obj;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2 = arr;

    REQUIRE_JSON(doc2, "[\"hello\"]");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "42");

    JsonDocument doc2 = doc1.as<JsonVariant>();

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }
}

TEST_CASE("JsonDocument assignment") {
  SECTION("Copy assignment reallocates when capacity is smaller") {
    JsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(8);

    doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Copy assignment reallocates when capacity is larger") {
    JsonDocument doc1(100);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(1234);

    doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Assign from JsonObject") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(4096);
    doc2 = obj;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(4096);
    doc2 = arr;

    REQUIRE_JSON(doc2, "[\"hello\"]");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "42");

    JsonDocument doc2(4096);
    doc2 = doc1.as<JsonVariant>();

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from MemberProxy") {
    JsonDocument doc1(200);
    doc1["value"] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1["value"];

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from ElementProxy") {
    JsonDocument doc1(200);
    doc1[0] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1[0];

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == 4096);
  }
}
