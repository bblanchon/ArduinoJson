#include "CppUnitTest.h"
#include "StringBuilder.h"
#include "JsonValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonValueTests)
    {
        char buffer[1024];
        size_t returnValue;

    public:

        TEST_METHOD(Null)
        {
            write((char*)0);

            assertReturns(4);
            assertResultIs("null");
        }

        TEST_METHOD(EmptyString)
        {
            write("");

            assertReturns(2);
            assertResultIs("\"\"");
        }

        TEST_METHOD(QuotationMark)
        {
            write("\"");
            assertReturns(4);
            assertResultIs("\"\\\"\"");
        }

        TEST_METHOD(ReverseSolidus)
        {
            write("\\");
            assertReturns(4);
            assertResultIs("\"\\\\\"");
        }

        TEST_METHOD(Solidus)
        {
            write("/");
            assertReturns(3);
            assertResultIs("\"/\""); // but the JSON format allows \/
        }

        TEST_METHOD(Backspace)
        {
            write("\b");
            assertReturns(4);
            assertResultIs("\"\\b\"");
        }

        TEST_METHOD(Formfeed)
        {
            write("\f");
            assertReturns(4);
            assertResultIs("\"\\f\"");
        }

        TEST_METHOD(Newline)
        {
            write("\n");
            assertReturns(4);
            assertResultIs("\"\\n\"");
        }

        TEST_METHOD(CarriageReturn)
        {
            write("\r");
            assertReturns(4);
            assertResultIs("\"\\r\"");
        }    

        TEST_METHOD(HorizontalTab)
        {
            write("\t");
            assertReturns(4);
            assertResultIs("\"\\t\"");
        }

        TEST_METHOD(Double)
        {
            write(3.14);
            assertReturns(4);
            assertResultIs("3.14");
        }

        TEST_METHOD(Integer)
        {
            write(314);
            assertReturns(3);
            assertResultIs("314");
        }

        TEST_METHOD(Short)
        {
            write((short)314);
            assertReturns(3);
            assertResultIs("314");
        }

        TEST_METHOD(Long)
        {
            write(314L);
            assertReturns(3);
            assertResultIs("314");
        }

        template<typename T>
        void write(T value)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            returnValue = JsonValue(value).printTo(sb);
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