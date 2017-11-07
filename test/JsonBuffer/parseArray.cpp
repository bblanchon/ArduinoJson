// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonBuffer::parseArray()") {
  DynamicJsonBuffer jb;

  SECTION("EmptyArray") {
    JsonArray& arr = jb.parseArray("[]");

    REQUIRE(arr.success());
    REQUIRE(0 == arr.size());
  }

  SECTION("MissingOpeningBracket") {
    JsonArray& arr = jb.parseArray("]");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("ArrayWithNoEnd") {
    JsonArray& arr = jb.parseArray("[");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("EmptyArrayWithLeadingSpaces") {
    JsonArray& arr = jb.parseArray("  []");

    REQUIRE(arr.success());
    REQUIRE(0 == arr.size());
  }

  SECTION("Garbage") {
    JsonArray& arr = jb.parseArray("%*$£¤");

    REQUIRE_FALSE(arr.success());
  }

  SECTION("OneInteger") {
    JsonArray& arr = jb.parseArray("[42]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("OneIntegerWithSpacesBefore") {
    JsonArray& arr = jb.parseArray("[ \t\r\n42]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("OneIntegerWithSpaceAfter") {
    JsonArray& arr = jb.parseArray("[42 \t\r\n]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 42);
  }

  SECTION("TwoIntegers") {
    JsonArray& arr = jb.parseArray("[42,84]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == 42);
    REQUIRE(arr[1] == 84);
  }

  SECTION("TwoDoubles") {
    JsonArray& arr = jb.parseArray("[4.2,1e2]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == 4.2);
    REQUIRE(arr[1] == 1e2);
  }

  SECTION("UnsignedLong") {
    JsonArray& arr = jb.parseArray("[4294967295]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == 4294967295UL);
  }

  SECTION("TwoBooleans") {
    JsonArray& arr = jb.parseArray("[true,false]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == true);
    REQUIRE(arr[1] == false);
  }

  SECTION("TwoNulls") {
    JsonArray& arr = jb.parseArray("[null,null]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0].as<char*>() == 0);
    REQUIRE(arr[1].as<char*>() == 0);
  }

  SECTION("TwoStringsDoubleQuotes") {
    JsonArray& arr = jb.parseArray("[ \"hello\" , \"world\" ]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("TwoStringsSingleQuotes") {
    JsonArray& arr = jb.parseArray("[ 'hello' , 'world' ]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("TwoStringsNoQuotes") {
    JsonArray& arr = jb.parseArray("[ hello , world ]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("EmptyStringsDoubleQuotes") {
    JsonArray& arr = jb.parseArray("[\"\",\"\"]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("EmptyStringSingleQuotes") {
    JsonArray& arr = jb.parseArray("[\'\',\'\']");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("EmptyStringNoQuotes") {
    JsonArray& arr = jb.parseArray("[,]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "");
    REQUIRE(arr[1] == "");
  }

  SECTION("ClosingDoubleQuoteMissing") {
    JsonArray& arr = jb.parseArray("[\"]");

    REQUIRE_FALSE(arr.success());
  }

  SECTION("ClosingSignleQuoteMissing") {
    JsonArray& arr = jb.parseArray("[\']");

    REQUIRE_FALSE(arr.success());
  }

  SECTION("StringWithEscapedChars") {
    JsonArray& arr = jb.parseArray("[\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "1\"2\\3/4\b5\f6\n7\r8\t9");
  }

  SECTION("StringWithUnterminatedEscapeSequence") {
    JsonArray& arr = jb.parseArray("\"\\\0\"", 4);
    REQUIRE_FALSE(arr.success());
  }

  SECTION("CCommentBeforeOpeningBracket") {
    JsonArray& arr = jb.parseArray("/*COMMENT*/  [\"hello\"]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentAfterOpeningBracket") {
    JsonArray& arr = jb.parseArray("[/*COMMENT*/ \"hello\"]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentBeforeClosingBracket") {
    JsonArray& arr = jb.parseArray("[\"hello\"/*COMMENT*/]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentAfterClosingBracket") {
    JsonArray& arr = jb.parseArray("[\"hello\"]/*COMMENT*/");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CCommentBeforeComma") {
    JsonArray& arr = jb.parseArray("[\"hello\"/*COMMENT*/,\"world\"]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CCommentAfterComma") {
    JsonArray& arr = jb.parseArray("[\"hello\",/*COMMENT*/ \"world\"]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CppCommentBeforeOpeningBracket") {
    JsonArray& arr = jb.parseArray("//COMMENT\n\t[\"hello\"]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentAfterOpeningBracket") {
    JsonArray& arr = jb.parseArray("[//COMMENT\n\"hello\"]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentBeforeClosingBracket") {
    JsonArray& arr = jb.parseArray("[\"hello\"//COMMENT\r\n]");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentAfterClosingBracket") {
    JsonArray& arr = jb.parseArray("[\"hello\"]//COMMENT\n");

    REQUIRE(arr.success());
    REQUIRE(1 == arr.size());
    REQUIRE(arr[0] == "hello");
  }

  SECTION("CppCommentBeforeComma") {
    JsonArray& arr = jb.parseArray("[\"hello\"//COMMENT\n,\"world\"]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("CppCommentAfterComma") {
    JsonArray& arr = jb.parseArray("[\"hello\",//COMMENT\n\"world\"]");

    REQUIRE(arr.success());
    REQUIRE(2 == arr.size());
    REQUIRE(arr[0] == "hello");
    REQUIRE(arr[1] == "world");
  }

  SECTION("InvalidCppComment") {
    JsonArray& arr = jb.parseArray("[/COMMENT\n]");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("InvalidComment") {
    JsonArray& arr = jb.parseArray("[/*/\n]");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("UnfinishedCComment") {
    JsonArray& arr = jb.parseArray("[/*COMMENT]");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("EndsInCppComment") {
    JsonArray& arr = jb.parseArray("[//COMMENT");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("AfterClosingStar") {
    JsonArray& arr = jb.parseArray("[/*COMMENT*");
    REQUIRE_FALSE(arr.success());
  }

  SECTION("DeeplyNested") {
    JsonArray& arr =
        jb.parseArray("[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]");
    REQUIRE(arr.success());
  }
}
