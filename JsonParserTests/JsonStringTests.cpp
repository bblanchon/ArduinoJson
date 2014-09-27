/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Parser;

namespace ArduinoJsonParserTests
{
    TEST_CLASS(JsonStringTests)
    {
        const char* actual;
        char json[256];
        JsonParser<32> parser;

    public:

        TEST_METHOD(EmptyString)
        {
            whenInputIs("");
            outputMustBe(0);
        }

        TEST_METHOD(JustOneQuote)
        {
            whenInputIs("\"");
            outputMustBe(0);
        }

        TEST_METHOD(SimpleString)
        {
            whenInputIs("\"Hi!\"");
            outputMustBe("Hi!");
        }

        TEST_METHOD(EscapedQuote)
        {
            whenInputIs("\"12\\\"34\"");  // ie 12\"34
            outputMustBe("12\"34");
        }

        TEST_METHOD(EscapedReverseSolidus)
        {
            whenInputIs("\"12\\\\34\""); // ie 12\\34
            outputMustBe("12\\34");
        }

        TEST_METHOD(EscapedSolidus)
        {
            whenInputIs("\"12\\/34\"");
            outputMustBe("12/34");
        }

        TEST_METHOD(EscapedBackspace)
        {
            whenInputIs("\"12\\b34\"");
            outputMustBe("12\b34");
        }

        TEST_METHOD(EscapedFormfeed)
        {
            whenInputIs("\"12\\f34\"");
            outputMustBe("12\f34");
        }
        
        TEST_METHOD(EscapedNewline)
        {
            whenInputIs("\"12\\n34\""); 
            outputMustBe("12\n34");
        }

        TEST_METHOD(EscapedCarriageReturn)
        {
            whenInputIs("\"12\\r34\"");
            outputMustBe("12\r34");
        }

        TEST_METHOD(EscapedTab)
        {
            whenInputIs("\"12\\t34\"");
            outputMustBe("12\t34");
        }

        TEST_METHOD(AllEscapedCharsTogether)
        {
            whenInputIs("\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");
            outputMustBe("1\"2\\3/4\b5\f6\n7\r8\t9");
        }

    private:       

        void whenInputIs(const char* input)
        {
            strcpy(json, input);
            actual = parser.parse(json);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, actual);
        }
    };
}
