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

TEST_F(JsonValueTests, IntegersAreCopiedByValue)
{
    jsonValue1 = 123;
    jsonValue2 = jsonValue1;
    jsonValue1 = 456;

    EXPECT_EQ(123, (int) jsonValue2);
}

TEST_F(JsonValueTests, DoublesAreCopiedByValue)
{
    jsonValue1 = 123.45;
    jsonValue2 = jsonValue1;
    jsonValue1 = 456.78;

    EXPECT_EQ(123.45, (double) jsonValue2);
}

TEST_F(JsonValueTests, BooleansAreCopiedByValue)
{
    jsonValue1 = true;
    jsonValue2 = jsonValue1;
    jsonValue1 = false;

    EXPECT_TRUE((bool) jsonValue2);
}

TEST_F(JsonValueTests, StringsAreCopiedByValue)
{
    jsonValue1 = "hello";
    jsonValue2 = jsonValue1;
    jsonValue1 = "world";

    EXPECT_STREQ("hello", (const char*) jsonValue2);
}


TEST_F(JsonValueTests, ObjectsAreCopiedByReference)
{
    JsonObject object = json.createObject();

    jsonValue1 = object;
    jsonValue2 = jsonValue1;

    object["hello"] = "world";
    jsonValue1 = 0;

    EXPECT_EQ(1, ((JsonObject) jsonValue2).size());
}

TEST_F(JsonValueTests, ArraysAreCopiedByReference)
{
    JsonArray array = json.createArray();

    jsonValue1 = array;
    jsonValue2 = jsonValue1;
    jsonValue1 = 0;
    
    array.add("world");

    EXPECT_EQ(1, ((JsonObject) jsonValue2).size());
}