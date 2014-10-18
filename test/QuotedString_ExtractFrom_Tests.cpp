#include <gtest/gtest.h>
#include <ArduinoJson/Internals/QuotedString.h>

using namespace ArduinoJson::Internals;

class QuotedString_ExtractFrom_Tests : public testing::Test
{
protected:
    void whenInputIs(const char* json)
    {
        strcpy(_jsonString, json);
        _result = QuotedString::extractFrom(_jsonString, &_endp);
    }

    void outputMustBe(const char* expected)
    {
        EXPECT_STREQ(expected, _result);
        EXPECT_EQ(_endp, _result + )
    }

private:
    char _jsonString[256];
    char* _result;
    char* _endp;
};


TEST_F(QuotedString_ExtractFrom_Tests, EmptyDoubleQuotedString)
{
    whenInputIs("\"\"");
    outputMustBe("");
}

TEST_F(QuotedString_ExtractFrom_Tests, EmptySingleQuotedString)
{
    whenInputIs("''");
    outputMustBe("");
}

TEST_F(QuotedString_ExtractFrom_Tests, SimpleDoubleQuotedString)
{
    whenInputIs("\"hello world\"");
    outputMustBe("hello world");
}

TEST_F(QuotedString_ExtractFrom_Tests, CurlyBraces)
{
    whenInputIs("\"{hello:world}\"");
    outputMustBe("{hello:world}");
}

TEST_F(QuotedString_ExtractFrom_Tests, SquareBraquets)
{
    whenInputIs("\"[hello,world]\"");
    outputMustBe("[hello,world]");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedDoubleQuote)
{
    whenInputIs("\"hello \\\"world\\\"\"");
    outputMustBe("hello \"world\"");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedSingleQuote)
{
    whenInputIs("\"hello \\\'world\\\'\"");
    outputMustBe("hello 'world'");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedSolidus)
{
    whenInputIs("\"hello \\/world\\/\"");
    outputMustBe("hello /world/");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedReverseSolidus)
{
    whenInputIs("\"hello \\\\world\\\\\"");
    outputMustBe("hello \\world\\");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedBackspace)
{
    whenInputIs("\"hello \\bworld\\b\"");
    outputMustBe("hello \bworld\b");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedFormfeed)
{
    whenInputIs("\"hello \\fworld\\f\"");
    outputMustBe("hello \fworld\f");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedNewline)
{
    whenInputIs("\"hello \\nworld\\n\"");
    outputMustBe("hello \nworld\n");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedCarriageReturn)
{
    whenInputIs("\"hello \\rworld\\r\"");
    outputMustBe("hello \rworld\r");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedTab)
{
    whenInputIs("\"hello \\tworld\\t\"");
    outputMustBe("hello \tworld\t");
}

TEST_F(QuotedString_ExtractFrom_Tests, AllEscapedCharsTogether)
{
    whenInputIs("\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");
    outputMustBe("1\"2\\3/4\b5\f6\n7\r8\t9");
}