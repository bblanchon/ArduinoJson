// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void REQUIRE_JSON(JsonDocument& doc, const std::string& expected) {
  std::string json;
  serializeJson(doc, json);
  REQUIRE(json == expected);
}

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

    doc1 = doc2;

    REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
  }

  SECTION("Contructor") {
    SECTION("Copy constructor") {
      StaticJsonDocument<200> doc1;
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      StaticJsonDocument<200> doc2 = doc1;

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Construct from StaticJsonDocument of different size") {
      StaticJsonDocument<300> doc1;
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      StaticJsonDocument<200> doc2 = doc1;

      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Construct from DynamicJsonDocument") {
      DynamicJsonDocument doc1(4096);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      StaticJsonDocument<200> doc2 = doc1;

      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Construct from JsonObject") {
      DynamicJsonDocument doc1(4096);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      StaticJsonDocument<200> doc2 = doc1.as<JsonObject>();

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Construct from JsonArray") {
      DynamicJsonDocument doc1(4096);
      deserializeJson(doc1, "[\"hello\",\"world\"]");

      StaticJsonDocument<200> doc2 = doc1.as<JsonArray>();

      deserializeJson(doc1, "[\"HELLO\",\"WORLD\"]");
      REQUIRE_JSON(doc2, "[\"hello\",\"world\"]");
    }

    SECTION("Construct from JsonVariant") {
      DynamicJsonDocument doc1(4096);
      deserializeJson(doc1, "42");

      StaticJsonDocument<200> doc2 = doc1.as<JsonVariant>();

      REQUIRE_JSON(doc2, "42");
    }
  }

  SECTION("Assignment") {
    SECTION("Copy assignment") {
      StaticJsonDocument<200> doc1, doc2;
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      doc2 = doc1;

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Assign from StaticJsonDocument of different capacity") {
      StaticJsonDocument<200> doc1;
      StaticJsonDocument<300> doc2;
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      doc2 = doc1;

      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Assign from DynamicJsonDocument") {
      StaticJsonDocument<200> doc1;
      DynamicJsonDocument doc2(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      doc2 = doc1;

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Assign from JsonArray") {
      StaticJsonDocument<200> doc1;
      DynamicJsonDocument doc2(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "[\"hello\",\"world\"]");

      doc2 = doc1.as<JsonArray>();

      deserializeJson(doc1, "[\"HELLO\",\"WORLD\"]");
      REQUIRE_JSON(doc2, "[\"hello\",\"world\"]");
    }

    SECTION("Assign from JsonArrayConst") {
      StaticJsonDocument<200> doc1;
      DynamicJsonDocument doc2(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "[\"hello\",\"world\"]");

      doc2 = doc1.as<JsonArrayConst>();

      deserializeJson(doc1, "[\"HELLO\",\"WORLD\"]");
      REQUIRE_JSON(doc2, "[\"hello\",\"world\"]");
    }

    SECTION("Assign from JsonObject") {
      StaticJsonDocument<200> doc1;
      DynamicJsonDocument doc2(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      doc2 = doc1.as<JsonObject>();

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Assign from JsonObjectConst") {
      StaticJsonDocument<200> doc1;
      DynamicJsonDocument doc2(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "{\"hello\":\"world\"}");

      doc2 = doc1.as<JsonObjectConst>();

      deserializeJson(doc1, "{\"HELLO\":\"WORLD\"}");
      REQUIRE_JSON(doc2, "{\"hello\":\"world\"}");
    }

    SECTION("Assign from JsonVariant") {
      DynamicJsonDocument doc1(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "42");

      StaticJsonDocument<200> doc2;
      doc2 = doc1.as<JsonVariant>();

      REQUIRE_JSON(doc2, "42");
    }

    SECTION("Assign from JsonVariantConst") {
      DynamicJsonDocument doc1(4096);
      doc1.to<JsonVariant>().set(666);
      deserializeJson(doc1, "42");

      StaticJsonDocument<200> doc2;
      doc2 = doc1.as<JsonVariantConst>();

      REQUIRE_JSON(doc2, "42");
    }
  }
}
