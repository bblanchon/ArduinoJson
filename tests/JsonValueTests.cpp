#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonValueTests : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        jsonValue1 = json.createValue();
        jsonValue2 = json.createValue();
    }

    StaticJsonBuffer<42> json;
    JsonValue jsonValue1;
    JsonValue jsonValue2;
};


TEST_F(JsonValueTests, CanStoreInteger)
{
    jsonValue1 = 123;

    EXPECT_EQ(123, (int) jsonValue1);
}

TEST_F(JsonValueTests, CanStoreDouble)
{
    jsonValue1 = 123.45;

    EXPECT_EQ(123.45, (double) jsonValue1);
}

TEST_F(JsonValueTests, CanStoreTrue)
{
    jsonValue1 = true;
    EXPECT_TRUE((bool) jsonValue1);
}

TEST_F(JsonValueTests, CanStoreFalse)
{
    jsonValue1 = false;
    EXPECT_FALSE((bool) jsonValue1);
}

TEST_F(JsonValueTests, CanStoreString)
{
    jsonValue1 = "hello";
 
    EXPECT_STREQ("hello", (const char*) jsonValue1);
}

TEST_F(JsonValueTests, CanStoreObject)
{
    JsonObject innerObject1 = json.createObject();

    jsonValue1 = innerObject1;

    EXPECT_EQ(innerObject1, (JsonObject) jsonValue1);
}

TEST_F(JsonValueTests, CanCopyInteger)
{
    jsonValue1 = 123;
    jsonValue2 = jsonValue1;
    jsonValue1 = 456;

    EXPECT_EQ(456, (int) jsonValue1);
    EXPECT_EQ(123, (int) jsonValue2);
}