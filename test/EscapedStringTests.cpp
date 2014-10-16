#include <gtest/gtest.h>

#include <ArduinoJson/Internals/EscapedString.h>
#include <ArduinoJson/Internals/StringBuilder.h>

using namespace ArduinoJson::Internals;

class EscapedStringTests : public testing::Test
{
protected:
    void whenInputIs(const char* input)
    {
        StringBuilder sb(buffer, sizeof(buffer));  
        returnValue = EscapedString::printTo(input, &sb);
    }

    void outputMustBe(const char* expected)
    {
        EXPECT_STREQ(expected, buffer);
        EXPECT_EQ(strlen(expected), returnValue);
    }

private:
    char buffer[1024];
    size_t returnValue;
    
};

TEST_F(EscapedStringTests, Null)
{
    whenInputIs(0);
    outputMustBe("null");
}

TEST_F(EscapedStringTests, EmptyString)
{
    whenInputIs("");
    outputMustBe("\"\"");
}

TEST_F(EscapedStringTests, QuotationMark)
{
    whenInputIs("\"");
    outputMustBe("\"\\\"\"");
}

TEST_F(EscapedStringTests, ReverseSolidus)
{
    whenInputIs("\\");
    outputMustBe("\"\\\\\"");
}

TEST_F(EscapedStringTests, Solidus)
{
    whenInputIs("/");
    outputMustBe("\"/\""); // but the JSON format allows \/
}

TEST_F(EscapedStringTests, Backspace)
{
    whenInputIs("\b");
    outputMustBe("\"\\b\"");
}

TEST_F(EscapedStringTests, Formfeed)
{
    whenInputIs("\f");
    outputMustBe("\"\\f\"");
}

TEST_F(EscapedStringTests, Newline)
{
    whenInputIs("\n");
    outputMustBe("\"\\n\"");
}

TEST_F(EscapedStringTests, CarriageReturn)
{
    whenInputIs("\r");
    outputMustBe("\"\\r\"");
}

TEST_F(EscapedStringTests, HorizontalTab)
{
    whenInputIs("\t");
    outputMustBe("\"\\t\"");
}