/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Generator;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<2> arr;
        char buffer[256];

    public:
        
        TEST_METHOD(Empty)
        {
            outputMustBe("[]");
        }

        TEST_METHOD(Null)
        {
            add((char*)0);

            outputMustBe("[null]");
        }

        TEST_METHOD(OneString)
        {
            add("hello");

            outputMustBe("[\"hello\"]");
        }

        TEST_METHOD(TwoStrings)
        {
            add("hello");
            add("world");

            outputMustBe("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            add("hello");
            add("world");
            add("lost");

            outputMustBe("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneDoubleDefaultDigits)
        {
            add(3.14159265358979323846);
            outputMustBe("[3.14]");
        }

        TEST_METHOD(OneDoubleFourDigits)
        {
            add<4>(3.14159265358979323846);
            outputMustBe("[3.1416]");
        }

        TEST_METHOD(OneInteger)
        {
            add(1);

            outputMustBe("[1]");
        }

        TEST_METHOD(TwoIntegers)
        {
            add(1);
            add(2);

            outputMustBe("[1,2]");
        }

        TEST_METHOD(OneIntegerOverCapacity)
        {
            add(1);
            add(2);
            add(3);

            outputMustBe("[1,2]");
        }

        TEST_METHOD(OneTrue)
        {
            add(true);

            outputMustBe("[true]");
        }

        TEST_METHOD(OneFalse)
        {
            add(false);

            outputMustBe("[false]");
        }

        TEST_METHOD(TwoBooleans)
        {
            add(false);
            add(true);

            outputMustBe("[false,true]");
        }

        TEST_METHOD(OneBooleanOverCapacity)
        {
            add(false);
            add(true);
            add(false);

            outputMustBe("[false,true]");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
           addNested(JsonArray<1>());
            outputMustBe("[[]]");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            addNested(JsonHashTable<1>());
            outputMustBe("[{}]");
        }

        TEST_METHOD(OneNestedArrayWithOneInteger)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(1);

            addNested(nestedArray);

            outputMustBe("[[1]]");
        }

    private:

        void addNested(Printable& value)
        {
            arr.add<Printable&>(value);
        }

        template<typename T>
        void add(T value)
        {
            arr.add(value);
        }

        template<int DIGITS>
        void add(double value)
        {
            arr.add<DIGITS>(value);
        }

        void outputMustBe(const char* expected)
        {      
            size_t n = arr.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), n);
        }
    };
}