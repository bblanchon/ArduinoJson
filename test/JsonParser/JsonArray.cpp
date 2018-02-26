// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(JsonArray&)") {
  DynamicJsonArray arr;

  SECTION("EmptyArray") {
    bool success = deserializeJson(arr, "[]");

    REQUIRE(success == true);
    REQUIRE(0 == arr.size());
  }

  SECTION("MissingOpeningBracket") {
    bool success = deserializeJson(arr, "]");
    REQUIRE_FALSE(success == true);
  }

  SECTION("ArrayWithNoEnd") {
    bool success = deserializeJson(arr, "[");
    REQUIRE_FALSE(success == true);
  }

  SECTION("EmptyArrayWithLeadingSpaces") {
    bool success = deserializeJson(arr, "  []");

    REQUIRE(success == true);
    REQUIRE(0 == arr.size());
  }

  SECTION("Garbage") {
    bool success = deserializeJson(arr, "%*$£¤");

    REQUIRE_FALSE(success == true);
  }

  SECTION("OneInteger") {
    bool success = deserializeJson(arr, "[42]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("OneIntegerWithSpacesBefore") {
    bool success = deserializeJson(arr, "[ \t\r\n42]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("OneIntegerWithSpaceAfter") {
    bool success = deserializeJson(arr, "[42 \t\r\n]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("TwoIntegers") {
    bool success = deserializeJson(arr, "[42,84]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == 84);
  }

  SECTION("TwoDoubles") {
    bool success = deserializeJson(arr, "[4.2,1e2]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == 4.2);
    REQUIRE(arr[1] == 1e2);
  }

  SECTION("UnsignedLong") {
    bool success = deserializeJson(arr, "[4294967295]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 4294967295UL);
  }

  SECTION("TwoBooleans") {
    bool success = deserializeJson(arr, "[true,false]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == true);
    REQUIRE(arr[1] == false);
  }

  SECTION("TwoNulls") {
    bool success = deserializeJson(arr, "[null,null]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0].as<char *>() == 0);
    REQUIRE(arr[1].as<char *>() == 0);
  }

  SECTION("TwoStringsDoubleQuotes") {
    bool success = deserializeJson(arr, "[ \"hello\" , \"world\" ]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("TwoStringsSingleQuotes") {
    bool success = deserializeJson(arr, "[ 'hello' , 'world' ]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("TwoStringsNoQuotes") {
    bool success = deserializeJson(arr, "[ hello , world ]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("EmptyStringsDoubleQuotes") {
    bool success = deserializeJson(arr, "[\"\",\"\"]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("EmptyStringSingleQuotes") {
    bool success = deserializeJson(arr, "[\'\',\'\']");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("EmptyStringNoQuotes") {
    bool success = deserializeJson(arr, "[,]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("ClosingDoubleQuoteMissing") {
    bool success = deserializeJson(arr, "[\"]");

    REQUIRE_FALSE(success == true);
  }

  SECTION("ClosingSignleQuoteMissing") {
    bool success = deserializeJson(arr, "[\']");

    REQUIRE_FALSE(success == true);
  }

  SECTION("StringWithEscapedChars") {
    bool success =
        deserializeJson(arr, "[\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "1\"2\\3/4\b5\f6\n7\r8\t9");
  }

  SECTION("StringWithUnterminatedEscapeSequence") {
    bool success = deserializeJson(arr, "\"\\\0\"", 4);
    REQUIRE_FALSE(success == true);
  }

  SECTION("CCommentBeforeOpeningBracket") {
    bool success = deserializeJson(arr, "/*COMMENT*/  [\"hello\"]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentAfterOpeningBracket") {
    bool success = deserializeJson(arr, "[/*COMMENT*/ \"hello\"]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentBeforeClosingBracket") {
    bool success = deserializeJson(arr, "[\"hello\"/*COMMENT*/]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentAfterClosingBracket") {
    bool success = deserializeJson(arr, "[\"hello\"]/*COMMENT*/");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentBeforeComma") {
    bool success = deserializeJson(arr, "[\"hello\"/*COMMENT*/,\"world\"]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CCommentAfterComma") {
    bool success = deserializeJson(arr, "[\"hello\",/*COMMENT*/ \"world\"]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CppCommentBeforeOpeningBracket") {
    bool success = deserializeJson(arr, "//COMMENT\n\t[\"hello\"]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentAfterOpeningBracket") {
    bool success = deserializeJson(arr, "[//COMMENT\n\"hello\"]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentBeforeClosingBracket") {
    bool success = deserializeJson(arr, "[\"hello\"//COMMENT\r\n]");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentAfterClosingBracket") {
    bool success = deserializeJson(arr, "[\"hello\"]//COMMENT\n");

    REQUIRE(success == true);
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentBeforeComma") {
    bool success = deserializeJson(arr, "[\"hello\"//COMMENT\n,\"world\"]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CppCommentAfterComma") {
    bool success = deserializeJson(arr, "[\"hello\",//COMMENT\n\"world\"]");

    REQUIRE(success == true);
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("InvalidCppComment") {
    bool success = deserializeJson(arr, "[/COMMENT\n]");
    REQUIRE_FALSE(success == true);
  }

  SECTION("InvalidComment") {
    bool success = deserializeJson(arr, "[/*/\n]");
    REQUIRE_FALSE(success == true);
  }

  SECTION("UnfinishedCComment") {
    bool success = deserializeJson(arr, "[/*COMMENT]");
    REQUIRE_FALSE(success == true);
  }

  SECTION("EndsInCppComment") {
    bool success = deserializeJson(arr, "[//COMMENT");
    REQUIRE_FALSE(success == true);
  }

  SECTION("AfterClosingStar") {
    bool success = deserializeJson(arr, "[/*COMMENT*");
    REQUIRE_FALSE(success == true);
  }

  SECTION("DeeplyNested") {
    bool success = deserializeJson(
        arr, "[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]");
    REQUIRE(success == true);
  }

  SECTION("ObjectNestedInArray") {
    char jsonString[] =
        " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

    bool success = deserializeJson(arr, jsonString);

    JsonObject &object1 = arr[0];
    const JsonObject &object2 = arr[1];
    JsonObject &object3 = arr[2];

    REQUIRE(true == success);

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
