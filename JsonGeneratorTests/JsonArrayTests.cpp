#include "CppUnitTest.h"
#include "JsonArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{		
    TEST_CLASS(JsonArrayTests)
    {
        JsonArray<2> arr;
        
    public:
        
        TEST_METHOD(Empty)
        {
            assertJsonIs("[]");
        }

        TEST_METHOD(AddNull)
        {
            arr.add((char*)0);

            assertJsonIs("[null]");
        }

        TEST_METHOD(AddOneString)
        {
            arr.add("hello");

            assertJsonIs("[\"hello\"]");
        }

        TEST_METHOD(AddTwoStrings)
        {
            arr.add("hello");
            arr.add("world");

            assertJsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneStringOverCapacity)
        {
            arr.add("hello");
            arr.add("world");
            arr.add("lost");

            assertJsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneNumber)
        {
            arr.add(3.14);

            assertJsonIs("[3.14]");
        }

        TEST_METHOD(AddTwoNumbers)
        {
            arr.add(3.14);
            arr.add(2.72);

            assertJsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddOneNumberOverCapacity)
        {
            arr.add(3.14);
            arr.add(2.72);
            arr.add(1.41);

            assertJsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddTrue)
        {
            arr.add(true);

            assertJsonIs("[true]");
        }

        TEST_METHOD(AddFalse)
        {
            arr.add(false);

            assertJsonIs("[false]");
        }

        TEST_METHOD(AddTwoBooleans)
        {
            arr.add(false);
            arr.add(true);

            assertJsonIs("[false,true]");
        }

        TEST_METHOD(AddOneBooleanOverCapacity)
        {
            arr.add(false);
            arr.add(true);
            arr.add(false);

            assertJsonIs("[false,true]");
        }

        TEST_METHOD(AddOneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            arr.add(nestedArray);

            assertJsonIs("[[]]");
        }

        TEST_METHOD(AddOneNestedArrayWithOneItem)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(3.14);

            arr.add(nestedArray);

            assertJsonIs("[[3.14]]");
        }

    private:

        void assertJsonIs(const char* expected)
        {      
            char buffer[256];

            arr.writeTo(buffer, sizeof(buffer));

            Assert::AreEqual(expected, buffer);
        }
    };
}