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
                "[\n"
                " 1\n"
                "]");
        }

        TEST_METHOD(TwoElements)
        {
            whenInputIs("[1,2]");
            outputMustBe(
                "[\n"
                " 1,\n"
                " 2\n"
                "]");
        }

        TEST_METHOD(EmptyNestedArrays)
        {
            whenInputIs("[[],[]]");
            outputMustBe(
                "[\n"
                " [],\n"
                " []\n"
                "]");
        }

        TEST_METHOD(NestedArrays)
        {
            whenInputIs("[[1,2],[3,4]]");
            outputMustBe(
                "[\n"
                " [\n"
                "  1,\n"
                "  2\n"
                " ],\n"
                " [\n"
                "  3,\n"
                "  4\n"
                " ]\n"
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