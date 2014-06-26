#include "CppUnitTest.h"
#include "StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(StringBuilderAppendEscapedTests)
    {
        char buffer[16];
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
            assertResultIs("\"ABCDEFGHIJKLM\"");
        }
        /*
        TEST_METHOD(SpecialChars)
        {
            append("\\\"\b\f\n\r");
            assertResultIs("\\\\\\\"\\\b\\\f\\\n\\\r");
        }
        */
        void append(const char* s)
        {
            sb->appendEscaped(s);
        }

        void assertResultIs(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }
    };
}