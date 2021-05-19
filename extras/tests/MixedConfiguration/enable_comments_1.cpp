// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("Comments in arrays") {
  DynamicJsonDocument doc(2048);

  SECTION("Block comments") {
    SECTION("Before opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "/*COMMENT*/  [\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "[/*COMMENT*/ \"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"/*COMMENT*/]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"]/*COMMENT*/");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"/*COMMENT*/,\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\",/*COMMENT*/ \"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("/*/") {
      DeserializationError err = deserializeJson(doc, "[/*/\n]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("Unfinished comment") {
      DeserializationError err = deserializeJson(doc, "[/*COMMENT]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("Final slash missing") {
      DeserializationError err = deserializeJson(doc, "[/*COMMENT*]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Trailing comments") {
    SECTION("Before opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "//COMMENT\n\t[\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After opening bracket") {
      DeserializationError err = deserializeJson(doc, "[//COMMENT\n\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before closing bracket") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"//COMMENT\r\n]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("After closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"]//COMMENT\n");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SECTION("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"//COMMENT\n,\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("After comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\",//COMMENT\n\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SECTION("Invalid comment") {
      DeserializationError err = deserializeJson(doc, "[/COMMENT\n]");
      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("End document with comment") {
      DeserializationError err = deserializeJson(doc, "[//COMMENT");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }
}

TEST_CASE("Comments in objects") {
  DynamicJsonDocument doc(2048);

  SECTION("Block comments") {
    SECTION("Before opening brace") {
      DeserializationError err =
          deserializeJson(doc, "/*COMMENT*/ {\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After opening brace") {
      DeserializationError err =
          deserializeJson(doc, "{/*COMMENT*/\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"/*COMMENT*/:\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":/*COMMENT*/\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"/*COMMENT*/}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"}/*COMMENT*/");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\"/*COMMENT*/,\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }

    SECTION("After comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\",/*COMMENT*/\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }
  }

  SECTION("Trailing comments") {
    SECTION("Before opening brace") {
      DeserializationError err =
          deserializeJson(doc, "//COMMENT\n {\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After opening brace") {
      DeserializationError err =
          deserializeJson(doc, "{//COMMENT\n\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"//COMMENT\n:\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"://COMMENT\n\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"//COMMENT\n}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("After closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"}//COMMENT\n");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\"//COMMENT\n,\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }

    SECTION("After comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\",//COMMENT\n\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }
  }

  SECTION("Dangling slash") {
    SECTION("Before opening brace") {
      DeserializationError err = deserializeJson(doc, "/{\"hello\":\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("After opening brace") {
      DeserializationError err = deserializeJson(doc, "{/\"hello\":\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("Before colon") {
      DeserializationError err = deserializeJson(doc, "{\"hello\"/:\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("After colon") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":/\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("Before closing brace") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":\"world\"/}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("After closing brace") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":\"world\"}/");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SECTION("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"/,\"answer\":42}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SECTION("After comma") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\",/\"answer\":42}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }
  }
}

TEST_CASE("Comments alone") {
  DynamicJsonDocument doc(2048);

  SECTION("Just a trailing comment with no line break") {
    DeserializationError err = deserializeJson(doc, "// comment");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SECTION("Just a trailing comment with no a break") {
    DeserializationError err = deserializeJson(doc, "// comment\n");

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SECTION("Just a block comment") {
    DeserializationError err = deserializeJson(doc, "/*comment*/");

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SECTION("Just a slash") {
    DeserializationError err = deserializeJson(doc, "/");

    REQUIRE(err == DeserializationError::InvalidInput);
  }

  SECTION("Premature terminator") {
    DeserializationError err = deserializeJson(doc, "/* comment");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SECTION("Premature end on sized input") {
    DeserializationError err = deserializeJson(doc, "/* comment */", 10);

    REQUIRE(err == DeserializationError::IncompleteInput);
  }
}
