#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonArray_Parser_Tests : public testing::Test
{
protected:
    void parse(const char* json)
    {
        strcpy(_jsonString, json);
        _array = _jsonBuffer.parseArray(_jsonString);
    }

    StaticJsonBuffer<42> _jsonBuffer;
    JsonArray _array;
    char _jsonString[256];
};

TEST_F(JsonArray_Parser_Tests, EmptyArray)
{
    parse("[]");

    EXPECT_TRUE(_array.success());
    EXPECT_EQ(0, _array.size());
}

TEST_F(JsonArray_Parser_Tests, ArrayWithNoEnd)
{
    parse("[");

    EXPECT_FALSE(_array.success());
    EXPECT_EQ(0, _array.size());
}

TEST_F(JsonArray_Parser_Tests, EmptyArrayWithLeadingSpaces)
{
    parse("  []");

    EXPECT_TRUE(_array.success());
    EXPECT_EQ(0, _array.size());
}

TEST_F(JsonArray_Parser_Tests, Garbage)
{
    parse("%*$£¤");

    EXPECT_FALSE(_array.success());
    EXPECT_EQ(0, _array.size());
}

TEST_F(JsonArray_Parser_Tests, OneInteger)
{
    parse("[42]");

    EXPECT_TRUE(_array.success());
    EXPECT_EQ(1, _array.size());
    EXPECT_EQ(42, static_cast<int>(_array[0]));
}

TEST_F(JsonArray_Parser_Tests, OneIntegerWithSpacesBefore)
{
    parse("[ \t\r\n42]");

    EXPECT_TRUE(_array.success());
    EXPECT_EQ(1, _array.size());
    EXPECT_EQ(42, static_cast<int>(_array[0]));
}

TEST_F(JsonArray_Parser_Tests, OneIntegerWithSpaceAfter)
{
    parse("[42 \t\r\n]");

    EXPECT_TRUE(_array.success());
    EXPECT_EQ(1, _array.size());
    EXPECT_EQ(42, static_cast<int>(_array[0]));
}

TEST_F(JsonArray_Parser_Tests, TwoIntegers)
{
    parse("[42,84]");

    EXPECT_TRUE(_array.success());

    EXPECT_EQ(2, _array.size());
    EXPECT_EQ(42, static_cast<int>(_array[0]));
    EXPECT_EQ(84, static_cast<int>(_array[1]));
}