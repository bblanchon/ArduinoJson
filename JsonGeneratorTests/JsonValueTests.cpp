#include "CppUnitTest.h"
#include "StringBuilder.h"
#include "JsonValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonValueTests)
    {
        char buffer[20];
        StringBuilder* sb;

    public:

        TEST_METHOD_INITIALIZE(Initialize)
        {
            sb = new StringBuilder(buffer, sizeof(buffer));
        }
                
        TEST_METHOD(InitialState)
        {
            assertResultIs("");
        }

        TEST_METHOD(Null)
        {
            append((char*)0);
            assertResultIs("null");
        }

        TEST_METHOD(EmptyString)
        {
            append("");
            assertResultIs("\"\"");
        }

        TEST_METHOD(OneString)
        {
            append("ABCD");
            assertResultIs("\"ABCD\"");
        }

        TEST_METHOD(OneTwoStrings)
        {
            append("ABCD");
            append("EFGH");
            assertResultIs("\"ABCD\"\"EFGH\"");
        }

        TEST_METHOD(OverCapacity)
        {
            append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            assertResultIs("\"ABCDEFGHIJKLMNOPQR");

            append("");
            assertResultIs("\"ABCDEFGHIJKLMNOPQR");
        }

        TEST_METHOD(SpecialChars)
        {
            append("\\\"\b\f\n\r\t");
            assertResultIs("\"\\\\\\\"\\b\\f\\n\\r\\t\"");
        }

        TEST_METHOD(Number)
        {
            append(3.14);
            assertResultIs("3.14");
        }

        template<typename T>
        void append(T value)
        {
            JsonValue(value).writeTo(*sb);
        }

        void assertResultIs(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }
    };
}