#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonValueTests : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        jsonValue = json.createValue();
    }

    StaticJsonBuffer<42> json;
    JsonValue jsonValue;
};


TEST_F(JsonValueTests, CanStoreInteger)
{
    jsonValue = 123;

    EXPECT_EQ(123, (int) jsonValue);
}

TEST_F(JsonValueTests, CanStoreDouble)
{
    jsonValue = 123.45;

    EXPECT_EQ(123.45, (double) jsonValue);
}

TEST_F(JsonValueTests, CanStoreTrue)
{
    jsonValue = true;
    EXPECT_TRUE((bool) jsonValue);
}

TEST_F(JsonValueTests, CanStoreFalse)
{
    jsonValue = false;
    EXPECT_FALSE((bool) jsonValue);
}

TEST_F(JsonValueTests, CanStoreString)
{
    jsonValue = "hello";
 
    EXPECT_STREQ("hello", (const char*) jsonValue);
}

TEST_F(JsonValueTests, CanStoreObject)
{
    JsonObject innerObject1 = json.createObject();

    jsonValue = innerObject1;

    EXPECT_EQ(innerObject1, (JsonObject) jsonValue);
}