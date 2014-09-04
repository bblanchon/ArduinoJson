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

        TEST_METHOD(EscapedQuote)
        {
            whenInputIs("\\\"");
            outputMustBe("\"");
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