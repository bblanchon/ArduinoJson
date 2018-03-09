// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(JsonArray&)") {
  DynamicJsonArray arr;

  SECTION("An empty array") {
    JsonError err = deserializeJson(arr, "[]");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(0 == arr.size());
  }

  SECTION("Spaces") {
    SECTION("Before the opening bracket") {
      JsonError err = deserializeJson(arr, "  []");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(0 == arr.size());
    }

    SECTION("Before first value") {
      JsonError err = deserializeJson(arr, "[ \t\r\n42]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("After first value") {
      JsonError err = deserializeJson(arr, "[42 \t\r\n]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }
  }

  SECTION("Values types") {
    SECTION("On integer") {
      JsonError err = deserializeJson(arr, "[42]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 42);
    }

    SECTION("Two integers") {
      JsonError err = deserializeJson(arr, "[42,84]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 42);
      REQUIRE(arr[1] == 84);
    }

    SECTION("Double") {
      JsonError err = deserializeJson(arr, "[4.2,1e2]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == 4.2);
      REQUIRE(arr[1] == 1e2);
    }

    SECTION("Unsigned long") {
      JsonError err = deserializeJson(arr, "[4294967295]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == 4294967295UL);
    }

    SECTION("Boolean") {
      JsonError err = deserializeJson(arr, "[true,false]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == true);
      REQUIRE(arr[1] == false);
    }

    SECTION("Null") {
      JsonError err = deserializeJson(arr, "[null,null]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0].as<char *>() == 0);
      REQUIRE(arr[1].as<char *>() == 0);
    }
  }

  SECTION("Quotes") {
    SECTION("Double quotes") {
      JsonError err = deserializeJson(arr, "[ \"hello\" , \"world\" ]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Single quotes") {
      JsonError err = deserializeJson(arr, "[ 'hello' , 'world' ]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("No quotes") {
      JsonError err = deserializeJson(arr, "[ hello , world ]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Double quotes (empty strings)") {
      JsonError err = deserializeJson(arr, "[\"\",\"\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("Single quotes (empty strings)") {
      JsonError err = deserializeJson(arr, "[\'\',\'\']");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("No quotes (empty strings)") {
      JsonError err = deserializeJson(arr, "[,]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "");
      REQUIRE(arr[1] == "");
    }

    SECTION("Closing single quotes missing") {
      JsonError err = deserializeJson(arr, "[\"]");

      REQUIRE(err != JsonError::Ok);
    }

    SECTION("Closing double quotes missing") {
      JsonError err = deserializeJson(arr, "[\']");

      REQUIRE(err != JsonError::Ok);
    }
  }

  SECTION("Block comments") {
    SECTION("Before opening bracket") {
      JsonError err = deserializeJson(arr, "/*COMMENT*/  [\"hello\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      JsonError err = deserializeJson(arr, "[/*COMMENT*/ \"hello\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      JsonError err = deserializeJson(arr, "[\"hello\"/*COMMENT*/]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      JsonError err = deserializeJson(arr, "[\"hello\"]/*COMMENT*/");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      JsonError err = deserializeJson(arr, "[\"hello\"/*COMMENT*/,\"world\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      JsonError err = deserializeJson(arr, "[\"hello\",/*COMMENT*/ \"world\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("/*/") {
      JsonError err = deserializeJson(arr, "[/*/\n]");
      REQUIRE(err != JsonError::Ok);
    }

    SECTION("Unfinished comment") {
      JsonError err = deserializeJson(arr, "[/*COMMENT]");
      REQUIRE(err != JsonError::Ok);
    }

    SECTION("Final slash missing") {
      JsonError err = deserializeJson(arr, "[/*COMMENT*]");
      REQUIRE(err != JsonError::Ok);
    }
  }

  SECTION("Line comments") {
    SECTION("Before opening bracket") {
      JsonError err = deserializeJson(arr, "//COMMENT\n\t[\"hello\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      JsonError err = deserializeJson(arr, "[//COMMENT\n\"hello\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      JsonError err = deserializeJson(arr, "[\"hello\"//COMMENT\r\n]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      JsonError err = deserializeJson(arr, "[\"hello\"]//COMMENT\n");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      JsonError err = deserializeJson(arr, "[\"hello\"//COMMENT\n,\"world\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      JsonError err = deserializeJson(arr, "[\"hello\",//COMMENT\n\"world\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Invalid comment") {
      JsonError err = deserializeJson(arr, "[/COMMENT\n]");
      REQUIRE(err != JsonError::Ok);
    }

    SECTION("End document with comment") {
      JsonError err = deserializeJson(arr, "[//COMMENT");
      REQUIRE(err != JsonError::Ok);
    }
  }

  SECTION("Misc") {
    SECTION("Garbage") {
      JsonError err = deserializeJson(arr, "%*$£¤");

      REQUIRE(err != JsonError::Ok);
    }

    SECTION("The opening bracket is missing") {
      JsonError err = deserializeJson(arr, "]");

      REQUIRE(err != JsonError::Ok);  // TODO
    }

    SECTION("The closing bracket is missing") {
      JsonError err = deserializeJson(arr, "[");
      REQUIRE(err != JsonError::Ok);  // TODO
    }

    SECTION("Escape sequences") {
      JsonError err =
          deserializeJson(arr, "[\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"]");

      REQUIRE(err == JsonError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "1\"2\\3/4\b5\f6\n7\r8\t9");
    }

    SECTION("Unterminated escape sequence") {
      JsonError err = deserializeJson(arr, "\"\\\0\"", 4);
      REQUIRE(err != JsonError::Ok);
    }

    SECTION("Nested objects") {
      char jsonString[] =
          " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

      JsonError err = deserializeJson(arr, jsonString);

      JsonObject &object1 = arr[0];
      const JsonObject &object2 = arr[1];
      JsonObject &object3 = arr[2];

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
}
