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
            addValue((char*)0);

            jsonIs("[null]");
        }

        TEST_METHOD(AddOneString)
        {
            addValue("hello");

            jsonIs("[\"hello\"]");
        }

        TEST_METHOD(AddTwoStrings)
        {
            addValue("hello");
            addValue("world");

            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneStringOverCapacity)
        {
            addValue("hello");
            addValue("world");
            addValue("lost");

            jsonIs("[\"hello\",\"world\"]");
        }

        TEST_METHOD(AddOneNumber)
        {
            addValue(3.14);

            jsonIs("[3.14]");
        }

        TEST_METHOD(AddTwoNumbers)
        {
            addValue(3.14);
            addValue(2.72);

            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddOneNumberOverCapacity)
        {
            addValue(3.14);
            addValue(2.72);
            addValue(1.41);

            jsonIs("[3.14,2.72]");
        }

        TEST_METHOD(AddTrue)
        {
            addValue(true);

            jsonIs("[true]");
        }

        TEST_METHOD(AddFalse)
        {
            addValue(false);

            jsonIs("[false]");
        }

        TEST_METHOD(AddTwoBooleans)
        {
            addValue(false);
            addValue(true);

            jsonIs("[false,true]");
        }

        TEST_METHOD(AddOneBooleanOverCapacity)
        {
            addValue(false);
            addValue(true);
            addValue(false);

            jsonIs("[false,true]");
        }

        TEST_METHOD(AddOneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            
            addNested(nestedArray);

            jsonIs("[[]]");
        }

        TEST_METHOD(AddOneNestedArrayWithOneItem)
        {
            JsonArray<1> nestedArray;
            nestedArray.add(3.14);

            addNested(nestedArray);

            jsonIs("[[3.14]]");
        }

    private:

        JsonArray<2> arr;

        void addNested(JsonObjectBase& value)
        {
            arr.add(value);
        }

        template<typename T>
        void addValue(T value)
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