// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonObject_PrettyPrintTo_Tests : public testing::Test {
 public:
  JsonObject_PrettyPrintTo_Tests() : object(json.createObject()) {}

 protected:
  StaticJsonBuffer<300> json;
  JsonObject &object;

  void outputMustBe(const char *expected) {
    size_t n = object.prettyPrintTo(buffer, sizeof(buffer));
    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), n);
  }

 private:
  char buffer[256];
};

TEST_F(JsonObject_PrettyPrintTo_Tests, EmptyObject) { outputMustBe("{}"); }

TEST_F(JsonObject_PrettyPrintTo_Tests, OneMember) {
  object["key"] = "value";

  outputMustBe(
      "{\r\n"
      "  \"key\": \"value\"\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, TwoMembers) {
  object["key1"] = "value1";
  object["key2"] = "value2";

  outputMustBe(
      "{\r\n"
      "  \"key1\": \"value1\",\r\n"
      "  \"key2\": \"value2\"\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, EmptyNestedContainers) {
  object.createNestedObject("key1");
  object.createNestedArray("key2");

  outputMustBe(
      "{\r\n"
      "  \"key1\": {},\r\n"
      "  \"key2\": []\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, NestedContainers) {
  JsonObject &nested1 = object.createNestedObject("key1");
  nested1["a"] = 1;

  JsonArray &nested2 = object.createNestedArray("key2");
  nested2.add(2);

  outputMustBe(
      "{\r\n"
      "  \"key1\": {\r\n"
      "    \"a\": 1\r\n"
      "  },\r\n"
      "  \"key2\": [\r\n"
      "    2\r\n"
      "  ]\r\n"
      "}");
}
