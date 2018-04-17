// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize JSON array") {
  DynamicJsonDocument doc;

  SECTION("An empty array") {
    JsonError err = deserializeJson(doc, "[]");
    JsonArray& arr = doc.as<JsonArray>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(0 == arr.size());
  }

  SECTION("Spaces") {
    SECTION("Before the opening bracket") {
      JsonError err = deserializeJson(doc, "  []");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(0 == arr.size());
    }

    SECTION("Before first value") {
      JsonError err = deserializeJson(doc, "[ \t\r\n42]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("After first value") {
      JsonError err = deserializeJson(doc, "[42 \t\r\n]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }
  }

  SECTION("Values types") {
    SECTION("On integer") {
      JsonError err = deserializeJson(doc, "[42]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("Two integers") {
      JsonError err = deserializeJson(doc, "[42,84]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 42);
      REQUIRE(arr[1] == 84);
    }

    SECTION("Double") {
      JsonError err = deserializeJson(doc, "[4.2,1e2]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 4.2);
      REQUIRE(arr[1] == 1e2);
    }

    SECTION("Unsigned long") {
      JsonError err = deserializeJson(doc, "[4294967295]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 4294967295UL);
    }

    SECTION("Boolean") {
      JsonError err = deserializeJson(doc, "[true,false]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == true);
      REQUIRE(arr[1] == false);
    }

    SECTION("Null") {
      JsonError err = deserializeJson(doc, "[null,null]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0].as<char*>() == 0);
      REQUIRE(arr[1].as<char*>() == 0);
    }
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      JsonError err = deserializeJson(doc, "[ \"hello\" , \"world\" ]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Single quotes") {
      JsonError err = deserializeJson(doc, "[ 'hello' , 'world' ]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("No quotes") {
      JsonError err = deserializeJson(doc, "[ hello , world ]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Double quotes (empty strings)") {
      JsonError err = deserializeJson(doc, "[\"\",\"\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("Single quotes (empty strings)") {
      JsonError err = deserializeJson(doc, "[\'\',\'\']");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("No quotes (empty strings)") {
      JsonError err = deserializeJson(doc, "[,]");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("Closing single quotes missing") {
      JsonError err = deserializeJson(doc, "[\"]");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("Closing double quotes missing") {
      JsonError err = deserializeJson(doc, "[\']");

      REQUIRE(err == JsonError::InvalidInput);
    }
  }

  SECTION("Block comments") {
    SECTION("Before opening bracket") {
      JsonError err = deserializeJson(doc, "/*COMMENT*/  [\"hello\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      JsonError err = deserializeJson(doc, "[/*COMMENT*/ \"hello\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      JsonError err = deserializeJson(doc, "[\"hello\"/*COMMENT*/]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      JsonError err = deserializeJson(doc, "[\"hello\"]/*COMMENT*/");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      JsonError err = deserializeJson(doc, "[\"hello\"/*COMMENT*/,\"world\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      JsonError err = deserializeJson(doc, "[\"hello\",/*COMMENT*/ \"world\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("/*/") {
      JsonError err = deserializeJson(doc, "[/*/\n]");
      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("Unfinished comment") {
      JsonError err = deserializeJson(doc, "[/*COMMENT]");
      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("Final slash missing") {
      JsonError err = deserializeJson(doc, "[/*COMMENT*]");
      REQUIRE(err == JsonError::InvalidInput);
    }
  }

  SECTION("Line comments") {
    SECTION("Before opening bracket") {
      JsonError err = deserializeJson(doc, "//COMMENT\n\t[\"hello\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      JsonError err = deserializeJson(doc, "[//COMMENT\n\"hello\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      JsonError err = deserializeJson(doc, "[\"hello\"//COMMENT\r\n]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      JsonError err = deserializeJson(doc, "[\"hello\"]//COMMENT\n");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      JsonError err = deserializeJson(doc, "[\"hello\"//COMMENT\n,\"world\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      JsonError err = deserializeJson(doc, "[\"hello\",//COMMENT\n\"world\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Invalid comment") {
      JsonError err = deserializeJson(doc, "[/COMMENT\n]");
      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("End document with comment") {
      JsonError err = deserializeJson(doc, "[//COMMENT");
      REQUIRE(err == JsonError::InvalidInput);
    }
  }

  SECTION("Misc") {
    SECTION("Garbage") {
      JsonError err = deserializeJson(doc, "%*$£¤");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("The opening bracket is missing") {
      JsonError err = deserializeJson(doc, "]");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("The closing bracket is missing") {
      JsonError err = deserializeJson(doc, "[");

      REQUIRE(err == JsonError::InvalidInput);
    }

    SECTION("Escape sequences") {
      JsonError err =
          deserializeJson(doc, "[\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"]");
      JsonArray& arr = doc.as<JsonArray>();

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "1\"2\\3/4\b5\f6\n7\r8\t9");
    }

    SECTION("Nested objects") {
      char jsonString[] =
          " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

      JsonError err = deserializeJson(doc, jsonString);
      JsonArray& arr = doc.as<JsonArray>();

      JsonObject& object1 = arr[0];
      const JsonObject& object2 = arr[1];
      JsonObject& object3 = arr[2];

      REQUIRE(err == JsonError::Ok);

      REQUIRE(true == object1.success());
      REQUIRE(true == object2.success());
      REQUIRE(false == object3.success());

      REQUIRE(2 == object1.size());
      REQUIRE(2 == object2.size());
      REQUIRE(0 == object3.size());

      REQUIRE(1 == object1["a"].as<int>());
      REQUIRE(2 == object1["b"].as<int>());
      REQUIRE(3 == object2["c"].as<int>());
      REQUIRE(4 == object2["d"].as<int>());
      REQUIRE(0 == object3["e"].as<int>());
    }
  }

  SECTION("Should clear the JsonArray") {
    deserializeJson(doc, "[1,2,3,4]");
    deserializeJson(doc, "[]");
    JsonArray& arr = doc.as<JsonArray>();

    REQUIRE(arr.size() == 0);
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
  }
}
