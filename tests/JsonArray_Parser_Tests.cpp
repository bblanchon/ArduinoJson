#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonArray_Parser_Tests : public testing::Test
{
protected:
    StaticJsonBuffer<42> json;
};

TEST_F(JsonArray_Parser_Tests, EmptyArray)
{
    JsonArray array = json.parseArray("[]");

    EXPECT_TRUE(array.success());
    EXPECT_EQ(0, array.size());
}

TEST_F(JsonArray_Parser_Tests, EmptyArrayWithLeadingSpaces)
{
    JsonArray array = json.parseArray("  []");

    EXPECT_TRUE(array.success());
    EXPECT_EQ(0, array.size());
}

TEST_F(JsonArray_Parser_Tests, Garbage)
{
    JsonArray array = json.parseArray("%*$£¤");

    EXPECT_FALSE(array.success());
    EXPECT_EQ(0, array.size());
}

TEST_F(JsonArray_Parser_Tests, OneInteger)
{
    JsonArray array = json.parseArray("[42]");

    EXPECT_TRUE(array.success());
    EXPECT_EQ(1, array.size());
    EXPECT_EQ(42, static_cast<int>(array[0])); 
}