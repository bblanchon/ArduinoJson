#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
    public:
        
        TEST_METHOD(Empty)
        {
            jsonIs("[]");
        }

        TEST_METHOD(AddNull)
        {
            add((char*)0);

            jsonIs("[null]");
        }

        TEST_METHOD(AddOneString)
        {
            add("hello");

            jsonIs("[\"hello\"]");
        }

        TEST_METHOD(AddTwoStrings)
        {
            add("hello");
            add("world");

            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneStringOverCapacity)
        {
            add("hello");
            add("world");
            add("lost");

            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneNumber)
        {
            add(3.14);

            jsonIs("[3.14]");
        }

        TEST_METHOD(AddTwoNumbers)
        {
            add(3.14);
            add(2.72);

            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddOneNumberOverCapacity)
        {
            add(3.14);
            add(2.72);
            add(1.41);

            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddTrue)
        {
            add(true);

            jsonIs("[true]");
        }

        TEST_METHOD(AddFalse)
        {
            add(false);

            jsonIs("[false]");
        }

        TEST_METHOD(AddTwoBooleans)
        {
            add(false);
            add(true);

            jsonIs("[false,true]");
        }

        TEST_METHOD(AddOneBooleanOverCapacity)
        {
            add(false);
            add(true);
            add(false);

            jsonIs("[false,true]");
        }

        TEST_METHOD(AddOneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            add(nestedArray);

            jsonIs("[[]]");
        }

        TEST_METHOD(AddOneNestedArrayWithOneItem)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(3.14);

            add(nestedArray);

            jsonIs("[[3.14]]");
        }

    private:

        JsonArray<2> arr;

        template<typename T>
        void add(T value)
        {
            arr.add(value);
        }

        void jsonIs(const char* expected)
        {      
            char buffer[256];

            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}