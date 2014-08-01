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
    TEST_CLASS(JsonValue_Cast_Tests)
    {
        JsonValue value;

    public:

        TEST_METHOD(Bool)
        {
            setValueAndCheckCast(true);
            setValueAndCheckCast(false);
        }

        TEST_METHOD(Double)
        {
            setValueAndCheckCast(3.14156);
        }

        TEST_METHOD(Float)
        {
            setValueAndCheckCast(3.14f);
        }

        TEST_METHOD(Integer)
        {
            setValueAndCheckCast(42);
        }

        TEST_METHOD(Long)
        {
            setValueAndCheckCast(42L);
        }

        TEST_METHOD(String)
        {
            setValueAndCheckCast("hello");
        }

    private:

        template<typename T>
        void setValueAndCheckCast(T expected)
        {
            value = expected;
            T actual = value;
            Assert::AreEqual(expected, actual);
        }
    };
}