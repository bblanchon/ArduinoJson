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

TEST_F(JsonParser_String_Tests, EscapedSolidus)
{
    whenInputIs("\"hello \\/world\\/\"");
    outputMustBe("hello /world/");
}

TEST_F(JsonParser_String_Tests, EscapedReverseSolidus)
{
    whenInputIs("\"hello \\\\world\\\\\"");
    outputMustBe("hello \\world\\");
}

TEST_F(JsonParser_String_Tests, EscapedBackspace)
{
    whenInputIs("\"hello \\bworld\\b");
    outputMustBe("hello \bworld\b");
}

TEST_F(JsonParser_String_Tests, EscapedFormfeed)
{
    whenInputIs("\"hello \\fworld\\f");
    outputMustBe("hello \fworld\f");
}

TEST_F(JsonParser_String_Tests, EscapedNewline)
{
    whenInputIs("\"hello \\nworld\\n");
    outputMustBe("hello \nworld\n");
}

TEST_F(JsonParser_String_Tests, EscapedCarriageReturn)
{
    whenInputIs("\"hello \\rworld\\r");
    outputMustBe("hello \rworld\r");
}

TEST_F(JsonParser_String_Tests, EscapedTab)
{
    whenInputIs("\"hello \\tworld\\t");
    outputMustBe("hello \tworld\t");
}

TEST_F(JsonParser_String_Tests, AllEscapedCharsTogether)
{
    whenInputIs("\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");
    outputMustBe("1\"2\\3/4\b5\f6\n7\r8\t9");
}