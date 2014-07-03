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
            assertResultIs("null");
        }

        TEST_METHOD(EmptyString)
        {
            write("");
            assertResultIs("\"\"");
        }

        TEST_METHOD(QuotationMark)
        {
            write("\"");
            assertResultIs("\"\\\"\"");
        }

        TEST_METHOD(ReverseSolidus)
        {
            write("\\");
            assertResultIs("\"\\\\\"");
        }

        TEST_METHOD(Solidus)
        {
            write("/");
            assertResultIs("\"/\""); // but the JSON format allows \/
        }

        TEST_METHOD(Backspace)
        {
            write("\b");
            assertResultIs("\"\\b\"");
        }

        TEST_METHOD(Formfeed)
        {
            write("\f");
            assertResultIs("\"\\f\"");
        }

        TEST_METHOD(Newline)
        {
            write("\n");
            assertResultIs("\"\\n\"");
        }

        TEST_METHOD(CarriageReturn)
        {
            write("\r");
            assertResultIs("\"\\r\"");
        }    

        TEST_METHOD(HorizontalTab)
        {
            write("\t");
            assertResultIs("\"\\t\"");
        }

        TEST_METHOD(DoubleDefaultDigits)
        {
            write(3.14159265358979323846);
            assertResultIs("3.14");
        }

        TEST_METHOD(DoubleZeroDigits)
        {
            write(3.14159265358979323846, 0);
            assertResultIs("3");
        }

        TEST_METHOD(DoubleOneDigit)
        {
            write(3.14159265358979323846, 1);
            assertResultIs("3.1");
        }

        TEST_METHOD(DoubleTwoDigits)
        {
            write(3.14159265358979323846, 2);
            assertResultIs("3.14");
        }

        TEST_METHOD(Integer)
        {
            write(314);
            assertResultIs("314");
        }

        TEST_METHOD(Short)
        {
            write((short)314);
            assertResultIs("314");
        }

        TEST_METHOD(Long)
        {
            write(314L);
            assertResultIs("314");
        }

        template<typename T>
        void write(T value)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            returnValue = JsonValue(value).printTo(sb);
        }

        void write(double value, int digits)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            returnValue = JsonValue(value, digits).printTo(sb);
        }

        void assertResultIs(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), returnValue);
        }
    };
}