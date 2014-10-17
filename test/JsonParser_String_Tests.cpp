#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.h>
#include <ArduinoJson/JsonValue.h>

class JsonParser_String_Tests : public testing::Test
{
protected:
    void whenInputIs(const char* json)
    {
        strcpy(_jsonString, json);
        _result = _jsonBuffer.parseValue(_jsonString);
    }

    void outputMustBe(const char* expected)
    {
        EXPECT_STREQ(expected, _result);
    }

    char _jsonString[256];
    StaticJsonBuffer<42> _jsonBuffer;
    const char* _result;
};


TEST_F(JsonParser_String_Tests, EmptyString)
{
    whenInputIs("\"\"");
    outputMustBe("");
}

TEST_F(JsonParser_String_Tests, SimpleString)
{
    whenInputIs("\"hello world\"");
    outputMustBe("hello world");
}

TEST_F(JsonParser_String_Tests, CurlyBraces)
{
    whenInputIs("\"{hello:world}\"");
    outputMustBe("{hello:world}");
}

TEST_F(JsonParser_String_Tests, SquareBraquets)
{
    whenInputIs("\"[hello,world]\"");
    outputMustBe("[hello,world]");
}

TEST_F(JsonParser_String_Tests, EscapedDoubleQuote)
{
    whenInputIs("\"hello \\\"world\\\"\"");
    outputMustBe("hello \"world\"");
}

TEST_F(JsonParser_String_Tests, EscapedSingleQuote)
{
    whenInputIs("\"hello \\\'world\\\'\"");
    outputMustBe("hello 'world'");
}