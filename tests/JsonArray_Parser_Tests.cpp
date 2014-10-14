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

    EXPECT_EQ(0, array.size());
}