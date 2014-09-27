/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "EscapedString.h"
#include "StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Internals;

namespace JsonGeneratorTests
{
    TEST_CLASS(EscapedStringTests)
    {
        char buffer[1024];
        size_t returnValue;
        
    public:

        TEST_METHOD(Null)
        {
            whenInputIs(0);
            outputMustBe("null");
        }

        TEST_METHOD(EmptyString)
        {
            whenInputIs("");
            outputMustBe("\"\"");
        }

        TEST_METHOD(QuotationMark)
        {
            whenInputIs("\"");
            outputMustBe("\"\\\"\"");
        }

        TEST_METHOD(ReverseSolidus)
        {
            whenInputIs("\\");
            outputMustBe("\"\\\\\"");
        }

        TEST_METHOD(Solidus)
        {
            whenInputIs("/");
            outputMustBe("\"/\""); // but the JSON format allows \/
        }

        TEST_METHOD(Backspace)
        {
            whenInputIs("\b");
            outputMustBe("\"\\b\"");
        }

        TEST_METHOD(Formfeed)
        {
            whenInputIs("\f");
            outputMustBe("\"\\f\"");
        }

        TEST_METHOD(Newline)
        {
            whenInputIs("\n");
            outputMustBe("\"\\n\"");
        }

        TEST_METHOD(CarriageReturn)
        {
            whenInputIs("\r");
            outputMustBe("\"\\r\"");
        }

        TEST_METHOD(HorizontalTab)
        {
            whenInputIs("\t");
            outputMustBe("\"\\t\"");
        }

    private:
        void whenInputIs(const char* input)
        {
            StringBuilder sb(buffer, sizeof(buffer));  
            returnValue = EscapedString::printTo(input, sb);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), returnValue);
        }
    };
}
