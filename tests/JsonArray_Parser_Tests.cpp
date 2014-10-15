#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonArray_Parser_Tests : public testing::Test
{
protected:
    void whenInputIs(const char *json)
    {
        strcpy(_jsonString, json);
        _array = _jsonBuffer.parseArray(_jsonString);
    }

    void parseMustSucceed()
    {
        EXPECT_TRUE(_array.success());
    }

    void parseMustFail()
    {
        EXPECT_FALSE(_array.success());
        EXPECT_EQ(0, _array.size());
    }

    void sizeMustBe(int expected)
    {
        EXPECT_EQ(expected, _array.size());
    }

    template<typename T>
    void firstElementMustBe(T expected)
    {
        elementAtIndexMustBe(0, expected);
    }

    template<typename T>
    void secondElementMustBe(T expected)
    {
        elementAtIndexMustBe(1, expected);
    }

    template<typename T>
    void elementAtIndexMustBe(int index, T expected)
    {
        EXPECT_EQ(expected, static_cast<T>(_array[index]));
    }

    StaticJsonBuffer<42> _jsonBuffer;
    JsonArray _array;
    char _jsonString[256];
};

TEST_F(JsonArray_Parser_Tests, EmptyArray)
{
    whenInputIs("[]");

    parseMustSucceed();
    sizeMustBe(0);
}

TEST_F(JsonArray_Parser_Tests, ArrayWithNoEnd)
{
    whenInputIs("[");

    parseMustFail();
}

TEST_F(JsonArray_Parser_Tests, EmptyArrayWithLeadingSpaces)
{
    whenInputIs("  []");

    parseMustSucceed();
    sizeMustBe(0);
}

TEST_F(JsonArray_Parser_Tests, Garbage)
{
    whenInputIs("%*$£¤");

    parseMustFail();
}

TEST_F(JsonArray_Parser_Tests, OneInteger)
{
    whenInputIs("[42]");

    parseMustSucceed();
    sizeMustBe(1);
    firstElementMustBe(42);
}

TEST_F(JsonArray_Parser_Tests, OneIntegerWithSpacesBefore)
{
    whenInputIs("[ \t\r\n42]");

    parseMustSucceed();
    sizeMustBe(1);
    firstElementMustBe(42);
}

TEST_F(JsonArray_Parser_Tests, OneIntegerWithSpaceAfter)
{
    whenInputIs("[42 \t\r\n]");

    parseMustSucceed();
    sizeMustBe(1);
    firstElementMustBe(42);
}

TEST_F(JsonArray_Parser_Tests, TwoIntegers)
{
    whenInputIs("[42,84]");

    parseMustSucceed();
    sizeMustBe(2);
    firstElementMustBe(42);
    secondElementMustBe(84);
}

TEST_F(JsonArray_Parser_Tests, TwoDoubles)
{
    whenInputIs("[4.2,8.4]");

    parseMustSucceed();
    sizeMustBe(2);
    firstElementMustBe(4.2);
    secondElementMustBe(8.4);
}

TEST_F(JsonArray_Parser_Tests, TwoBooleans)
{
    whenInputIs("[true,false]");

    parseMustSucceed();
    sizeMustBe(2);
    firstElementMustBe(true);
    secondElementMustBe(false);
}

TEST_F(JsonArray_Parser_Tests, TwoNulls)
{
    const char* const nullCharPtr = 0;

    whenInputIs("[null,null]");

    parseMustSucceed();
    sizeMustBe(2);
    firstElementMustBe(nullCharPtr);
    secondElementMustBe(nullCharPtr);
}