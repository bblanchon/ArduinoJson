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
        size_t returnValue;

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

            assertReturns(4);
            assertResultIs("null");
        }

        TEST_METHOD(EmptyString)
        {
            append("");

            assertReturns(2);
            assertResultIs("\"\"");
        }

        TEST_METHOD(OneString)
        {
            append("ABCD");
            assertReturns(6);
            assertResultIs("\"ABCD\"");
        }

        TEST_METHOD(OneTwoStrings)
        {
            append("ABCD");
            assertReturns(6);

            append("EFGH");
            assertReturns(6);
            
            assertResultIs("\"ABCD\"\"EFGH\"");
        }

        TEST_METHOD(OverCapacity)
        {
            append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            assertReturns(19);
            assertResultIs("\"ABCDEFGHIJKLMNOPQR");

            append("ABC");
            assertReturns(0);
            assertResultIs("\"ABCDEFGHIJKLMNOPQR");
        }

        TEST_METHOD(SpecialChars)
        {
            append("\\\"\b\f\n\r\t");
            assertReturns(16);
            assertResultIs("\"\\\\\\\"\\b\\f\\n\\r\\t\"");
        }

        TEST_METHOD(Number)
        {
            append(3.14);
            assertReturns(4);
            assertResultIs("3.14");
        }

        template<typename T>
        void append(T value)
        {
            returnValue = JsonValue(value).writeTo(*sb);
        }

        void assertResultIs(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }

        void assertReturns(size_t expected)
        {
            Assert::AreEqual(expected, returnValue);
        }
    };
}