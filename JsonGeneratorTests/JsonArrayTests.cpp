/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ArduinoJson::Generator;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<2> array;
        char buffer[256];

    public:
        
        TEST_METHOD(Empty)
        {
            outputMustBe("[]");
        }

        TEST_METHOD(Null)
        {
            array.add(static_cast<char*>(0));

            outputMustBe("[null]");
        }

        TEST_METHOD(OneString)
        {
            array.add("hello");

            outputMustBe("[\"hello\"]");
        }

        TEST_METHOD(TwoStrings)
        {
            array.add("hello");
            array.add("world");

            outputMustBe("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            array.add("hello");
            array.add("world");
            array.add("lost");

            outputMustBe("[\"hello\",\"world\"]");
        }

        TEST_METHOD(OneDoubleDefaultDigits)
        {
            array.add(3.14159265358979323846);
            outputMustBe("[3.14]");
        }

        TEST_METHOD(OneDoubleFourDigits)
        {
            array.add<4>(3.14159265358979323846);
            outputMustBe("[3.1416]");
        }

        TEST_METHOD(OneInteger)
        {
            array.add(1);

            outputMustBe("[1]");
        }

        TEST_METHOD(TwoIntegers)
        {
            array.add(1);
            array.add(2);

            outputMustBe("[1,2]");
        }

        TEST_METHOD(OneIntegerOverCapacity)
        {
            array.add(1);
            array.add(2);
            array.add(3);

            outputMustBe("[1,2]");
        }

        TEST_METHOD(OneTrue)
        {
            array.add(true);

            outputMustBe("[true]");
        }

        TEST_METHOD(OneFalse)
        {
            array.add(false);

            outputMustBe("[false]");
        }

        TEST_METHOD(TwoBooleans)
        {
            array.add(false);
            array.add(true);

            outputMustBe("[false,true]");
        }

        TEST_METHOD(OneBooleanOverCapacity)
        {
            array.add(false);
            array.add(true);
            array.add(false);

            outputMustBe("[false,true]");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            array.add(nestedArray);

            outputMustBe("[[]]");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            JsonObject<1> nestedObject;

            array.add(nestedObject);

            outputMustBe("[{}]");
        }

        TEST_METHOD(OneNestedArrayWithOneInteger)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(1);

            array.add(nestedArray);

            outputMustBe("[[1]]");
        }

    private:

        void outputMustBe(const char* expected)
        {      
            size_t n = array.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
            Assert::AreEqual(strlen(expected), n);
        }
    };
}
