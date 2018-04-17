// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize JSON object") {
  DynamicJsonDocument doc;

  SECTION("An empty object") {
    JsonError err = deserializeJson(doc, "{}");
    JsonObject& obj = doc.as<JsonObject>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonObject>());
    REQUIRE(obj.size() == 0);
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      JsonError err = deserializeJson(doc, "{\"key\":\"value\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Single quotes") {
      JsonError err = deserializeJson(doc, "{'key':'value'}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes") {
      JsonError err = deserializeJson(doc, "{key:value}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("No quotes, allow underscore in key") {
      JsonError err = deserializeJson(doc, "{_k_e_y_:42}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["_k_e_y_"] == 42);
    }
  }

  SECTION("Spaces") {
    SECTION("Before the key") {
      JsonError err = deserializeJson(doc, "{ \"key\":\"value\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the key") {
      JsonError err = deserializeJson(doc, "{\"key\" :\"value\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the value") {
      JsonError err = deserializeJson(doc, "{\"key\": \"value\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("After the value") {
      JsonError err = deserializeJson(doc, "{\"key\":\"value\" }");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 1);
      REQUIRE(obj["key"] == "value");
    }

    SECTION("Before the colon") {
      JsonError err =
          deserializeJson(doc, "{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("After the colon") {
      JsonError err =
          deserializeJson(doc, "{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }
  }

  SECTION("Values types") {
    SECTION("String") {
      JsonError err =
          deserializeJson(doc, "{\"key1\":\"value1\",\"key2\":\"value2\"}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == "value1");
      REQUIRE(obj["key2"] == "value2");
    }

    SECTION("Integer") {
      JsonError err = deserializeJson(doc, "{\"key1\":42,\"key2\":-42}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 42);
      REQUIRE(obj["key2"] == -42);
    }

    SECTION("Double") {
      JsonError err = deserializeJson(doc, "{\"key1\":12.345,\"key2\":-7E89}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == 12.345);
      REQUIRE(obj["key2"] == -7E89);
    }

    SECTION("Booleans") {
      JsonError err = deserializeJson(doc, "{\"key1\":true,\"key2\":false}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"] == true);
      REQUIRE(obj["key2"] == false);
    }

    SECTION("Null") {
      JsonError err = deserializeJson(doc, "{\"key1\":null,\"key2\":null}");
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["key1"].as<char*>() == 0);
      REQUIRE(obj["key2"].as<char*>() == 0);
    }

    SECTION("Array") {
      char jsonString[] = " { \"ab\" : [ 1 , 2 ] , \"cd\" : [ 3 , 4 ] } ";

      JsonError err = deserializeJson(doc, jsonString);
      JsonObject& obj = doc.as<JsonObject>();

      JsonArray& array1 = obj["ab"];
      const JsonArray& array2 = obj["cd"];
      JsonArray& array3 = obj["ef"];

      REQUIRE(err == JsonError::Ok);

      REQUIRE(true == array1.success());
      REQUIRE(true == array2.success());
      REQUIRE(false == array3.success());

      REQUIRE(2 == array1.size());
      REQUIRE(2 == array2.size());
      REQUIRE(0 == array3.size());

      REQUIRE(1 == array1[0].as<int>());
      REQUIRE(2 == array1[1].as<int>());

      REQUIRE(3 == array2[0].as<int>());
      REQUIRE(4 == array2[1].as<int>());

      REQUIRE(0 == array3[0].as<int>());
    }
  }

  SECTION("Misc") {
    SECTION("The opening brace is missing") {
      JsonError err = deserializeJson(doc, "}");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("The closing brace is missing") {
      JsonError err = deserializeJson(doc, "{\"hello\":\"world\"");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("A quoted key without value") {
      JsonError err = deserializeJson(doc, "{\"key\"}");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("A non-quoted key without value") {
      JsonError err = deserializeJson(doc, "{key}");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("A dangling comma") {
      JsonError err = deserializeJson(doc, "{\"key1\":\"value1\",}");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("null as a key") {
      JsonError err = deserializeJson(doc, "{null:\"value\"}");

      REQUIRE(err == JsonError::Ok);
    }
  }

  SECTION("Should clear the JsonObject") {
    deserializeJson(doc, "{\"hello\":\"world\"}");
    deserializeJson(doc, "{}");
    JsonObject& obj = doc.as<JsonObject>();

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(obj.size() == 0);
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
