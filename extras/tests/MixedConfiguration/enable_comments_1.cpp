// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>

#include <doctest.h>

TEST_CASE("Comments in arrays") {
  JsonDocument doc;

  SUBCASE("Block comments") {
    SUBCASE("Before opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "/*COMMENT*/  [\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("After opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "[/*COMMENT*/ \"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("Before closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"/*COMMENT*/]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("After closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"]/*COMMENT*/");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"/*COMMENT*/,\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SUBCASE("After comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\",/*COMMENT*/ \"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SUBCASE("/*/") {
      DeserializationError err = deserializeJson(doc, "[/*/\n]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SUBCASE("Unfinished comment") {
      DeserializationError err = deserializeJson(doc, "[/*COMMENT]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SUBCASE("Final slash missing") {
      DeserializationError err = deserializeJson(doc, "[/*COMMENT*]");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SUBCASE("Trailing comments") {
    SUBCASE("Before opening bracket") {
      DeserializationError err =
          deserializeJson(doc, "//COMMENT\n\t[\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("After opening bracket") {
      DeserializationError err = deserializeJson(doc, "[//COMMENT\n\"hello\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("Before closing bracket") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"//COMMENT\r\n]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("After closing bracket") {
      DeserializationError err = deserializeJson(doc, "[\"hello\"]//COMMENT\n");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(1 == arr.size());
      REQUIRE(arr[0] == "hello");
    }

    SUBCASE("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\"//COMMENT\n,\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SUBCASE("After comma") {
      DeserializationError err =
          deserializeJson(doc, "[\"hello\",//COMMENT\n\"world\"]");
      JsonArray arr = doc.as<JsonArray>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(2 == arr.size());
      REQUIRE(arr[0] == "hello");
      REQUIRE(arr[1] == "world");
    }

    SUBCASE("Invalid comment") {
      DeserializationError err = deserializeJson(doc, "[/COMMENT\n]");
      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("End document with comment") {
      DeserializationError err = deserializeJson(doc, "[//COMMENT");
      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }
}

TEST_CASE("Comments in objects") {
  JsonDocument doc;

  SUBCASE("Block comments") {
    SUBCASE("Before opening brace") {
      DeserializationError err =
          deserializeJson(doc, "/*COMMENT*/ {\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After opening brace") {
      DeserializationError err =
          deserializeJson(doc, "{/*COMMENT*/\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"/*COMMENT*/:\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":/*COMMENT*/\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"/*COMMENT*/}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"}/*COMMENT*/");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\"/*COMMENT*/,\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }

    SUBCASE("After comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\",/*COMMENT*/\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }
  }

  SUBCASE("Trailing comments") {
    SUBCASE("Before opening brace") {
      DeserializationError err =
          deserializeJson(doc, "//COMMENT\n {\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After opening brace") {
      DeserializationError err =
          deserializeJson(doc, "{//COMMENT\n\"hello\":\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"//COMMENT\n:\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After colon") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\"://COMMENT\n\"world\"}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"//COMMENT\n}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("After closing brace") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"}//COMMENT\n");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\"//COMMENT\n,\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }

    SUBCASE("After comma") {
      DeserializationError err = deserializeJson(
          doc, "{\"hello\":\"world\",//COMMENT\n\"answer\":42}");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
      REQUIRE(obj["answer"] == 42);
    }
  }

  SUBCASE("Dangling slash") {
    SUBCASE("Before opening brace") {
      DeserializationError err = deserializeJson(doc, "/{\"hello\":\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("After opening brace") {
      DeserializationError err = deserializeJson(doc, "{/\"hello\":\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("Before colon") {
      DeserializationError err = deserializeJson(doc, "{\"hello\"/:\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("After colon") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":/\"world\"}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("Before closing brace") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":\"world\"/}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("After closing brace") {
      DeserializationError err = deserializeJson(doc, "{\"hello\":\"world\"}/");
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(obj["hello"] == "world");
    }

    SUBCASE("Before comma") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\"/,\"answer\":42}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }

    SUBCASE("After comma") {
      DeserializationError err =
          deserializeJson(doc, "{\"hello\":\"world\",/\"answer\":42}");

      REQUIRE(err == DeserializationError::InvalidInput);
    }
  }
}

TEST_CASE("Comments alone") {
  JsonDocument doc;

  SUBCASE("Just a trailing comment with no line break") {
    DeserializationError err = deserializeJson(doc, "// comment");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SUBCASE("Just a trailing comment with no a break") {
    DeserializationError err = deserializeJson(doc, "// comment\n");

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SUBCASE("Just a block comment") {
    DeserializationError err = deserializeJson(doc, "/*comment*/");

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SUBCASE("Just a slash") {
    DeserializationError err = deserializeJson(doc, "/");

    REQUIRE(err == DeserializationError::InvalidInput);
  }

  SUBCASE("Premature terminator") {
    DeserializationError err = deserializeJson(doc, "/* comment");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SUBCASE("Premature end on sized input") {
    DeserializationError err = deserializeJson(doc, "/* comment */", 10);

    REQUIRE(err == DeserializationError::IncompleteInput);
  }
}
