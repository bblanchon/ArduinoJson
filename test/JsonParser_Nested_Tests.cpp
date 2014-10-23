#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(JsonParser_Nested_Tests, ArrayNestedInObject)
{
    StaticJsonBuffer<42> jsonBuffer;
    char jsonString[] = "{\"key\":[1,2,3]}";
    
    JsonObject object = jsonBuffer.parseObject(jsonString);
    ASSERT_TRUE(object.success());
    
    JsonArray array = object["key"];
    ASSERT_TRUE(array.success());
    
    ASSERT_EQ(3, array.size());
    
    EXPECT_EQ(1, static_cast<int>(array[0]));
    EXPECT_EQ(2, static_cast<int>(array[1]));
    EXPECT_EQ(3, static_cast<int>(array[2]));
}
