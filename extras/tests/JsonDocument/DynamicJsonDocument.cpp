// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ARDUINOJSON_NAMESPACE::addPadding;

static void REQUIRE_JSON(JsonDocument& doc, const std::string& expected) {
  std::string json;
  serializeJson(doc, json);
  REQUIRE(json == expected);
}

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

TEST_CASE("DynamicJsonDocument constructor") {
  SECTION("Copy constructor") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    DynamicJsonDocument doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");

    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Construct from StaticJsonDocument") {
    StaticJsonDocument<200> doc1;
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    DynamicJsonDocument doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Construct from JsonObject") {
    StaticJsonDocument<200> doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    DynamicJsonDocument doc2 = obj;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonArray") {
    StaticJsonDocument<200> doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    DynamicJsonDocument doc2 = arr;

    REQUIRE_JSON(doc2, "[\"hello\"]");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonVariant") {
    StaticJsonDocument<200> doc1;
    deserializeJson(doc1, "42");

    DynamicJsonDocument doc2 = doc1.as<JsonVariant>();

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }
}

TEST_CASE("DynamicJsonDocument assignment") {
  SECTION("Copy assignment preserves the buffer when capacity is sufficient") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    DynamicJsonDocument doc2(doc1.capacity());
    doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Copy assignment realloc the buffer when capacity is insufficient") {
    DynamicJsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    DynamicJsonDocument doc2(8);

    REQUIRE(doc2.capacity() < doc1.memoryUsage());
    doc2 = doc1;
    REQUIRE(doc2.capacity() >= doc1.memoryUsage());

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
  }

  SECTION("Assign from StaticJsonDocument") {
    StaticJsonDocument<200> doc1;
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    DynamicJsonDocument doc2(4096);
    doc2.to<JsonVariant>().set(666);

    doc2 = doc1;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
  }

  SECTION("Assign from JsonObject") {
    StaticJsonDocument<200> doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    DynamicJsonDocument doc2(4096);
    doc2 = obj;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonArray") {
    StaticJsonDocument<200> doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    DynamicJsonDocument doc2(4096);
    doc2 = arr;

    REQUIRE_JSON(doc2, "[\"hello\"]");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonVariant") {
    StaticJsonDocument<200> doc1;
    deserializeJson(doc1, "42");

    DynamicJsonDocument doc2(4096);
    doc2 = doc1.as<JsonVariant>();

    REQUIRE_JSON(doc2, "42");
    REQUIRE(doc2.capacity() == 4096);
  }
}
