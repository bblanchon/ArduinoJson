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
    TEST_CLASS(PrettyPrint_Array_Tests)
    {
        char buffer[1024];
        size_t returnValue;

    public:
        
        TEST_METHOD(EmptyArray)
        {
            whenInputIs("[]");
            outputMustBe("[]");
        }

        TEST_METHOD(OneElement)
        {
            whenInputIs("[1]");
            outputMustBe(
                "[\r\n"
                "  1\r\n"
                "]");
        }

        TEST_METHOD(TwoElements)
        {
            whenInputIs("[1,2]");
            outputMustBe(
                "[\r\n"
                "  1,\r\n"
                "  2\r\n"
                "]");
        }

        TEST_METHOD(EmptyNestedArrays)
        {
            whenInputIs("[[],[]]");
            outputMustBe(
                "[\r\n"
                "  [],\r\n"
                "  []\r\n"
                "]");
        }

        TEST_METHOD(NestedArrays)
        {
            whenInputIs("[[1,2],[3,4]]");
            outputMustBe(
                "[\r\n"
                "  [\r\n"
                "    1,\r\n"
                "    2\r\n"
                "  ],\r\n"
                "  [\r\n"
                "    3,\r\n"
                "    4\r\n"
                "  ]\r\n"
                "]");
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
