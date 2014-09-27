/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonPrettyPrint.h"
#include "StringBuilder.h"

using namespace ArduinoJson::Internals;
using namespace ArduinoJson::Generator;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(PrettyPrint_String_Tests)
    {
        char buffer[1024];
        size_t returnValue;

    public:
        
        TEST_METHOD(EmptyString)
        {
            whenInputIs("");
            outputMustBe("");
        }

        TEST_METHOD(TrickyCharacters)
        {
            whenInputIs ("\":\\\"',\"");
            outputMustBe("\":\\\"',\"");
        }

        TEST_METHOD(OpeningCurlyBrace)
        {
            whenInputIs ("\"{\"");
            outputMustBe("\"{\"");
        }

        TEST_METHOD(OpeningSquareBrace)
        {
            whenInputIs("\"[\"");
            outputMustBe("\"[\"");
        }

        TEST_METHOD(ClosingCurlyBrace)
        {
            whenInputIs("\"}\"");
            outputMustBe("\"}\"");
        }

        TEST_METHOD(ClosingSquareBrace)
        {
            whenInputIs("\"]\"");
            outputMustBe("\"]\"");
        }
               
    private:

        void whenInputIs(const char input[])
        {
            StringBuilder sb(buffer, sizeof(buffer));
            IndentedPrint indentedPrint(sb);
            JsonPrettyPrint decorator(indentedPrint);

            returnValue = decorator.print(input);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), returnValue);
        }
    };
}
