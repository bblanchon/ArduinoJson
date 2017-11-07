// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonBuffer::parseObject()") {
  DynamicJsonBuffer jb;

  SECTION("EmptyObject") {
    JsonObject& obj = jb.parseObject("{}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 0);
  }

  SECTION("MissingOpeningBrace") {
    JsonObject& obj = jb.parseObject("}");
    REQUIRE_FALSE(obj.success());
  }

  SECTION("MissingClosingBrace") {
    JsonObject& obj = jb.parseObject("{");
    REQUIRE_FALSE(obj.success());
  }

  SECTION("MissingColonAndValue") {
    JsonObject& obj = jb.parseObject("{\"key\"}");
    REQUIRE_FALSE(obj.success());
  }

  SECTION("MissingQuotesAndColonAndValue") {
    JsonObject& obj = jb.parseObject("{key}");
    REQUIRE_FALSE(obj.success());
  }

  SECTION("OneString") {
    JsonObject& obj = jb.parseObject("{\"key\":\"value\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringSingleQuotes") {
    JsonObject& obj = jb.parseObject("{'key':'value'}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringNoQuotes") {
    JsonObject& obj = jb.parseObject("{key:value}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringSpaceBeforeKey") {
    JsonObject& obj = jb.parseObject("{ \"key\":\"value\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringSpaceAfterKey") {
    JsonObject& obj = jb.parseObject("{\"key\" :\"value\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringSpaceBeforeValue") {
    JsonObject& obj = jb.parseObject("{\"key\": \"value\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("OneStringSpaceAfterValue") {
    JsonObject& obj = jb.parseObject("{\"key\":\"value\" }");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 1);
    REQUIRE(obj["key"] == "value");
  }

  SECTION("TwoStrings") {
    JsonObject& obj =
        jb.parseObject("{\"key1\":\"value1\",\"key2\":\"value2\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == "value1");
    REQUIRE(obj["key2"] == "value2");
  }

  SECTION("TwoStringsSpaceBeforeComma") {
    JsonObject& obj =
        jb.parseObject("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == "value1");
    REQUIRE(obj["key2"] == "value2");
  }

  SECTION("TwoStringsSpaceAfterComma") {
    JsonObject& obj =
        jb.parseObject("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == "value1");
    REQUIRE(obj["key2"] == "value2");
  }

  SECTION("EndingWithAComma") {
    JsonObject& obj = jb.parseObject("{\"key1\":\"value1\",}");
    REQUIRE_FALSE(obj.success());
    REQUIRE(obj.size() == 0);
  }

  SECTION("TwoIntergers") {
    JsonObject& obj = jb.parseObject("{\"key1\":42,\"key2\":-42}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == 42);
    REQUIRE(obj["key2"] == -42);
  }

  SECTION("TwoDoubles") {
    JsonObject& obj = jb.parseObject("{\"key1\":12.345,\"key2\":-7E89}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == 12.345);
    REQUIRE(obj["key2"] == -7E89);
  }

  SECTION("TwoBooleans") {
    JsonObject& obj = jb.parseObject("{\"key1\":true,\"key2\":false}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"] == true);
    REQUIRE(obj["key2"] == false);
  }

  SECTION("TwoNulls") {
    JsonObject& obj = jb.parseObject("{\"key1\":null,\"key2\":null}");
    REQUIRE(obj.success());
    REQUIRE(obj.size() == 2);
    REQUIRE(obj["key1"].as<char*>() == 0);
    REQUIRE(obj["key2"].as<char*>() == 0);
  }

  SECTION("NullForKey") {
    JsonObject& obj = jb.parseObject("null:\"value\"}");
    REQUIRE_FALSE(obj.success());
  }
}
