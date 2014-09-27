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
    TEST_CLASS(PrettyPrint_Object_Tests)
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
                "{\r\n"
                "  \"key\": \"value\"\r\n"
                "}");
        }

        TEST_METHOD(TwoMembers)
        {
            whenInputIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
            outputMustBe(
                "{\r\n"
                "  \"key1\": \"value1\",\r\n"
                "  \"key2\": \"value2\"\r\n"
                "}");
        }

        TEST_METHOD(EmptyNestedObjects)
        {
            whenInputIs("{\"key1\":{},\"key2\":{}}");
            outputMustBe(
                "{\r\n"
                "  \"key1\": {},\r\n"
                "  \"key2\": {}\r\n"
                "}");
        }

        TEST_METHOD(NestedObjects)
        {
            whenInputIs("{\"key1\":{\"a\":1},\"key2\":{\"b\":2}}");
            outputMustBe(
                "{\r\n"
                "  \"key1\": {\r\n"
                "    \"a\": 1\r\n"
                "  },\r\n"
                "  \"key2\": {\r\n"
                "    \"b\": 2\r\n"
                "  }\r\n"
                "}");
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
