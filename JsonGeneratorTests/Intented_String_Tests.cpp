/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "IndentedPrintDecorator.h"
#include "StringBuilder.h"

using namespace ArduinoJson::Internals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(IntentedPrintTests)
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
               
    private:

        void whenInputIs(const char input[])
        {
            StringBuilder sb(buffer, sizeof(buffer));
            IndentedPrintDecorator decorator(sb);

            returnValue = decorator.print(input);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), returnValue);
        }
    };
}