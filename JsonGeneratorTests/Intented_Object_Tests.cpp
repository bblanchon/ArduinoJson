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
    TEST_CLASS(Indented_Object_Tests)
    {
        char buffer[1024];
        size_t returnValue;

    public:

        TEST_METHOD(EmptyObject)
        {
            whenInputIs("{}");
            outputMustBe("{}");
        }

        TEST_METHOD(OneMember)
        {
            whenInputIs("{\"key\":\"value\"}");
            outputMustBe(
                "{\n"
                " \"key\": \"value\"\n"
                "}");
        }

        TEST_METHOD(TwoMembers)
        {
            whenInputIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
            outputMustBe(
                "{\n"
                " \"key1\": \"value1\",\n"
                " \"key2\": \"value2\"\n"
                "}");
        }

        TEST_METHOD(EmptyNestedObjects)
        {
            whenInputIs("{\"key1\":{},\"key2\":{}}");
            outputMustBe(
                "{\n"
                " \"key1\": {},\n"
                " \"key2\": {}\n"
                "}");
        }

        TEST_METHOD(NestedObjects)
        {
            whenInputIs("{\"key1\":{\"a\":1},\"key2\":{\"b\":2}}");
            outputMustBe(
                "{\n"
                " \"key1\": {\n"
                "  \"a\": 1\n"
                " },\n"
                " \"key2\": {\n"
                "  \"b\": 2\n"
                " }\n"
                "}");
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