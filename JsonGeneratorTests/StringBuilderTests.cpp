#include "CppUnitTest.h"
#include "StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
	TEST_CLASS(StringBuilderTests)
	{
        char buffer[20];
        Print* sb;
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

        TEST_METHOD(OverCapacity)
        {
            write("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            assertReturns(19);
            assertResultIs("ABCDEFGHIJKLMNOPQRS");

            write("ABC");
            assertReturns(0);
            assertResultIs("ABCDEFGHIJKLMNOPQRS");
        }

        TEST_METHOD(EmptyString)
        {
            write("");

            assertReturns(0);
            assertResultIs("");
        }

        TEST_METHOD(OneString)
        {
            write("ABCD");
            assertReturns(4);
            assertResultIs("ABCD");
        }

        TEST_METHOD(TwoStrings)
        {
            write("ABCD");
            assertReturns(4);

            write("EFGH");
            assertReturns(4);

            assertResultIs("ABCDEFGH");
        }

    private:

        void write(const char* value)
        {
            returnValue = sb->print(value);
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