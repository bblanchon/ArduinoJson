#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>

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

TEST_F(JsonArray_Parser_Tests, Garbage)
{
    JsonArray array = json.parseArray("%*$£¤");

    EXPECT_FALSE(array.success());
    EXPECT_EQ(0, array.size());
}