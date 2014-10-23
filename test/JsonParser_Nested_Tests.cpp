#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(JsonParser_Nested_Tests, ArrayNestedInObject)
{
    StaticJsonBuffer<42> jsonBuffer;
    char jsonString[] = " { \"ab\" : [ 1 , 2 ] , \"cd\" : [ 3 , 4 ] } ";
    
    JsonObject object = jsonBuffer.parseObject(jsonString);
    JsonArray array1 = object["ab"];
    JsonArray array2 = object["cd"];
    
    ASSERT_TRUE(object.success());
    
    ASSERT_TRUE(array1.success());
    ASSERT_TRUE(array2.success());

    ASSERT_EQ(2, array1.size());
    ASSERT_EQ(2, array2.size());
    
    EXPECT_EQ(1, static_cast<int>(array1[0]));
    EXPECT_EQ(2, static_cast<int>(array1[1]));

    EXPECT_EQ(3, static_cast<int>(array2[0]));
    EXPECT_EQ(4, static_cast<int>(array2[1]));
}

TEST(JsonParser_Nested_Tests, ObjectNestedInArray)
{
    StaticJsonBuffer<42> jsonBuffer;
    char jsonString[] = " [ { \"a\" : 1 , \"b\" : 2 } , { \"c\" : 3 , \"d\" : 4 } ] ";
    
    JsonArray array = jsonBuffer.parseArray(jsonString);
    JsonObject object1 = array[0];
    JsonObject object2 = array[1];
    
    ASSERT_TRUE(array.success());
    
    ASSERT_TRUE(object1.success());
    ASSERT_TRUE(object2.success());
    
    ASSERT_EQ(2, object1.size());
    ASSERT_EQ(2, object2.size());
    
    EXPECT_EQ(1, static_cast<int>(object1["a"]));
    EXPECT_EQ(2, static_cast<int>(object1["b"]));
    EXPECT_EQ(3, static_cast<int>(object2["c"]));
    EXPECT_EQ(4, static_cast<int>(object2["d"]));
}