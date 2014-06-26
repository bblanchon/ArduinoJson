#include "CppUnitTest.h"
#include "StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(StringBuilderTests)
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

        TEST_METHOD(AppendEmptyString)
        {
            append("");
            assertResultIs("");
        }

        TEST_METHOD(AppendOneString)
        {
            append("ABCD");
            assertResultIs("ABCD");
        }

        TEST_METHOD(AppendOneTwoStrings)
        {
            append("ABCD");
            append("EFGH");
            assertResultIs("ABCDEFGH");
        }

        TEST_METHOD(AppendSpecialChars)
        {
            append("\\\"\/\b\f\n\r");
            assertResultIs("\\\"\/\b\f\n\r");
        }

        void append(const char* s)
        {
            sb->append(s);
        }

        void assertResultIs(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
        }
    };
}