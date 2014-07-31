/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "StringBuilder.h"
#include "JsonValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Internals;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonValueTests)
    {
        char buffer[1024];
        size_t returnValue;

    public:

        TEST_METHOD(String)
        {
            whenInputIs("hello");
            outputMustBe("\"hello\"");
        }

        TEST_METHOD(Float)
        {
            whenInputIs(3.1415f);
            outputMustBe("3.14");
        }

        TEST_METHOD(DoubleZeroDigits)
        {
            whenInputIs<0>(3.14159265358979323846);
            outputMustBe("3");
        }

        TEST_METHOD(DoubleOneDigit)
        {
            whenInputIs<1>(3.14159265358979323846);
            outputMustBe("3.1");
        }

        TEST_METHOD(DoubleTwoDigits)
        {
            whenInputIs<2>(3.14159265358979323846);
            outputMustBe("3.14");
        }
        
        TEST_METHOD(Integer)
        {
            whenInputIs(314);
            outputMustBe("314");
        }

        TEST_METHOD(Char)
        {
            whenInputIs('A');
            outputMustBe("65");
        }

        TEST_METHOD(Short)
        {
            whenInputIs((short)314);
            outputMustBe("314");
        }

        TEST_METHOD(Long)
        {
            whenInputIs(314159265L);
            outputMustBe("314159265");
        }

    private:

        template<int DIGITS>
        void whenInputIs(double value)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            JsonValue jsonValue;
            jsonValue.set<DIGITS>(value);
            returnValue = jsonValue.printTo(sb);
        }

        template<typename T>
        void whenInputIs(T value)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            JsonValue jsonValue;
            jsonValue = value;
            returnValue = jsonValue.printTo(sb);
        }

        void outputMustBe(const char* expected)
        {
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), returnValue);
        }
    };
}