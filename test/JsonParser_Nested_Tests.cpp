// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(JsonParser_Nested_Tests, ArrayNestedInObject) {
  DynamicJsonBuffer jsonBuffer;
  char jsonString[] = " { \"ab\" : [ 1 , 2 ] , \"cd\" : [ 3 , 4 ] } ";

  JsonObject &object = jsonBuffer.parseObject(jsonString);
  JsonArray &array1 = object["ab"];
  JsonArray &array2 = object["cd"];

  ASSERT_TRUE(object.success());

  ASSERT_TRUE(array1.success());
  ASSERT_TRUE(array2.success());

  ASSERT_EQ(2, array1.size());
  ASSERT_EQ(2, array2.size());

  EXPECT_EQ(1, array1[0].as<int>());
  EXPECT_EQ(2, array1[1].as<int>());

  EXPECT_EQ(3, array2[0].as<int>());
  EXPECT_EQ(4, array2[1].as<int>());
}

TEST(JsonParser_Nested_Tests, ObjectNestedInArray) {
  DynamicJsonBuffer jsonBuffer;
  char jsonString[] =
      " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";

  JsonArray &array = jsonBuffer.parseArray(jsonString);
  JsonObject &object1 = array[0];
  JsonObject &object2 = array[1];

  ASSERT_TRUE(array.success());

  ASSERT_TRUE(object1.success());
  ASSERT_TRUE(object2.success());

  ASSERT_EQ(2, object1.size());
  ASSERT_EQ(2, object2.size());

  EXPECT_EQ(1, object1["a"].as<int>());
  EXPECT_EQ(2, object1["b"].as<int>());
  EXPECT_EQ(3, object2["c"].as<int>());
  EXPECT_EQ(4, object2["d"].as<int>());
}
