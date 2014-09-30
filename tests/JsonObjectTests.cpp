#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonObjectTests : public ::testing::Test
{
protected:
    virtual void SetUp() 
    {
        object = json.createObject();
    }

    StaticJsonBuffer<42> json;
    JsonObject object;
};

TEST_F(JsonObjectTests, Grow_WhenValuesAreAdded)
{   
    object["hello"];
    EXPECT_EQ(1, object.size());

    object["world"];
    EXPECT_EQ(2, object.size());
}

TEST_F(JsonObjectTests, DoNotGrow_WhenSameValueIsAdded)
{
    object["hello"];
    EXPECT_EQ(1, object.size());

    object["hello"];
    EXPECT_EQ(1, object.size());
}

TEST_F(JsonObjectTests, Shrink_WhenValuesAreRemoved)
{
    object["hello"];
    object["world"];

    object.remove("hello");
    EXPECT_EQ(1, object.size());

    object.remove("world");
    EXPECT_EQ(0, object.size());
}

TEST_F(JsonObjectTests, DoNotShrink_WhenRemoveIsCalledWithAWrongKey)
{
    object["hello"];
    object["world"];

    object.remove(":-P");

    EXPECT_EQ(2, object.size());
}

TEST_F(JsonObjectTests, CanStoreIntegers)
{
    object["hello"] = 123;
    object["world"] = 456;

    EXPECT_EQ(123, (int) object["hello"]);
    EXPECT_EQ(456, (int) object["world"]);
}

TEST_F(JsonObjectTests, CanStoreDoubles)
{
    object["hello"] = 123.45;
    object["world"] = 456.78;

    EXPECT_EQ(123.45, (double) object["hello"]);
    EXPECT_EQ(456.78, (double) object["world"]);
}

TEST_F(JsonObjectTests, CanStoreBooleans)
{
    object["hello"] = true;
    object["world"] = false;

    EXPECT_TRUE((bool) object["hello"]);
    EXPECT_FALSE((bool) object["world"]);
}

TEST_F(JsonObjectTests, CanStoreStrings)
{
    object["hello"] = "h3110";
    object["world"] = "w0r1d";

    EXPECT_STREQ("h3110", (const char*) object["hello"]);
    EXPECT_STREQ("w0r1d", (const char*) object["world"]);
}

TEST_F(JsonObjectTests, CanStoreInnerObjects)
{
    JsonObject innerObject1 = json.createObject();
    JsonObject innerObject2 = json.createObject();

    object["hello"] = innerObject1;
    object["world"] = innerObject2;

    EXPECT_EQ(innerObject1, (JsonObject) object["hello"]);
    EXPECT_EQ(innerObject2, (JsonObject) object["world"]);
}