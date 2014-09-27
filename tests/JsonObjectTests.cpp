#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

TEST(JsonObjectTests, WhenValueIsAdded_ThenSizeIsIncreasedByOne)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"];
    EXPECT_EQ(1, object.size());

    object["world"];
    EXPECT_EQ(2, object.size());
}

TEST(JsonObjectTests, WhenTheSameValueIsAddedTwice_ThenSizeIsOnlyIncreasedByOne)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"];
    EXPECT_EQ(1, object.size());

    object["hello"];
    EXPECT_EQ(1, object.size());
}

TEST(JsonObjectTests, GivenAnIntegerStored_WhenRetreivingTheValue_ThenTheValueIsTheSame)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"] = 123;
    object["world"] = 456;

    EXPECT_EQ(123, (int) object["hello"]);
    EXPECT_EQ(456, (int) object["world"]);
}

TEST(JsonObjectTests, GivenAnDoubleStored_WhenRetreivingTheValue_ThenTheValueIsTheSame)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"] = 123.45;
    object["world"] = 456.78;

    EXPECT_EQ(123.45, (double) object["hello"]);
    EXPECT_EQ(456.78, (double) object["world"]);
}


TEST(JsonObjectTests, GivenABooleanStored_WhenRetreivingTheValue_ThenTheValueIsTheSame)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"] = true;
    object["world"] = false;

    EXPECT_TRUE((bool) object["hello"]);
    EXPECT_FALSE((bool) object["world"]);
}