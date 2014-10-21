#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

class JsonParser_Object_Test : public testing::Test
{
protected:


};

TEST_F(JsonParser_Object_Test, EmptyObject)
{
    StaticJsonBuffer<10> jsonBuffer;
    char jsonString[] = "{}";
    JsonObject object = jsonBuffer.parseObject(jsonString);

    EXPECT_TRUE(object.success());
}