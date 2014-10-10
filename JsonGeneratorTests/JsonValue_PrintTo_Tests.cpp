/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "StringBuilder.h"
#include "JsonValue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Generator;
using namespace ArduinoJson::Internals;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonValue_PrintTo_Tests)
    {
        char buffer[1024];
        size_t returnValue;

    public:

        TEST_METHOD(String)
        {
            setValueTo("hello");
            outputMustBe("\"hello\"");
        }

        TEST_METHOD(ZeroFloat)
        {
            setValueTo(0.0f);
            outputMustBe("0.0");
        }

        TEST_METHOD(Float)
        {
            setValueTo(3.1415f);
            outputMustBe("3.14");
        }

        TEST_METHOD(DoubleZeroDigits)
        {
            setValueTo<0>(3.14159265358979323846);
            outputMustBe("3");
        }

        TEST_METHOD(DoubleOneDigit)
        {
            setValueTo<1>(3.14159265358979323846);
            outputMustBe("3.1");
        }

        TEST_METHOD(DoubleTwoDigits)
        {
            setValueTo<2>(3.14159265358979323846);
            outputMustBe("3.14");
        }
        
        TEST_METHOD(Integer)
        {
            setValueTo(314);
            outputMustBe("314");
        }

        TEST_METHOD(Char)
        {
            setValueTo('A');
            outputMustBe("65");
        }

        TEST_METHOD(Short)
        {
            setValueTo(static_cast<short>(314));
            outputMustBe("314");
        }

        TEST_METHOD(Long)
        {
            setValueTo(314159265L);
            outputMustBe("314159265");
        }

    private:

        template<int DIGITS>
        void setValueTo(double value)
        {
            StringBuilder sb(buffer, sizeof(buffer));
            JsonValue jsonValue;
            jsonValue.set<DIGITS>(value);
            returnValue = jsonValue.printTo(sb);
        }

        template<typename T>
        void setValueTo(T value)
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
