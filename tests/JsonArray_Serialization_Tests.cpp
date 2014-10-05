/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include <gtest/gtest.h>
#include <JsonArray.h>
#include <JsonObject.h>
#include <StaticJsonBuffer.h>

class JsonArray_Serialization_Tests : testing::Test 
{
protected:
    virtual void SetUp()
    {
        array = json.createArray();
    }

    void outputMustBe(const char* expected)
    {      
        size_t n = array.printTo(buffer, sizeof(buffer));
        EXPECT_STREQ(expected, buffer);
        EXPECT_EQ(strlen(expected), n);
    }

private:
    JsonArray array;
    char buffer[256];
    StaticJsonBuffer<32> json;
};
/*
TEST_F(JsonArray_Serialization_Tests, Empty)
{
    outputMustBe("[]");
}

TEST_F(JsonArray_Serialization_Tests, Null)
{
    array.add((char*) 0);

    outputMustBe("[null]");
}

TEST_F(JsonArray_Serialization_Tests, OneString)
{
    array.add("hello");

    outputMustBe("[\"hello\"]");
}

TEST_F(JsonArray_Serialization_Tests, TwoStrings)
{
    array.add("hello");
    array.add("world");

    outputMustBe("[\"hello\",\"world\"]");
}

TEST_F(JsonArray_Serialization_Tests, OneStringOverCapacity)
{
    array.add("hello");
    array.add("world");
    array.add("lost");

    outputMustBe("[\"hello\",\"world\"]");
}

TEST_F(JsonArray_Serialization_Tests, OneDoubleDefaultDigits)
{
    array.add(3.14159265358979323846);
    outputMustBe("[3.14]");
}

TEST_F(JsonArray_Serialization_Tests, OneDoubleFourDigits)
{
    array.add<4>(3.14159265358979323846);
    outputMustBe("[3.1416]");
}

TEST_F(JsonArray_Serialization_Tests, OneInteger)
{
    array.add(1);

    outputMustBe("[1]");
}

TEST_F(JsonArray_Serialization_Tests, TwoIntegers)
{
    array.add(1);
    array.add(2);

    outputMustBe("[1,2]");
}

TEST_F(JsonArray_Serialization_Tests, OneIntegerOverCapacity)
{
    array.add(1);
    array.add(2);
    array.add(3);

    outputMustBe("[1,2]");
}

TEST_F(JsonArray_Serialization_Tests, OneTrue)
{
    array.add(true);

    outputMustBe("[true]");
}

TEST_F(JsonArray_Serialization_Tests, OneFalse)
{
    array.add(false);

    outputMustBe("[false]");
}

TEST_F(JsonArray_Serialization_Tests, TwoBooleans)
{
    array.add(false);
    array.add(true);

    outputMustBe("[false,true]");
}

TEST_F(JsonArray_Serialization_Tests, OneBooleanOverCapacity)
{
    array.add(false);
    array.add(true);
    array.add(false);

    outputMustBe("[false,true]");
}

TEST_F(JsonArray_Serialization_Tests, OneEmptyNestedArray)
{
    JsonArray<1> nestedArray;
    
    array.add(nestedArray);

    outputMustBe("[[]]");
}

TEST_F(JsonArray_Serialization_Tests, OneEmptyNestedHash)
{
    JsonObject<1> nestedObject;

    array.add(nestedObject);

    outputMustBe("[{}]");
}

TEST_F(JsonArray_Serialization_Tests, OneNestedArrayWithOneInteger)
{
    JsonArray<1> nestedArray;
    nestedArray.add(1);

    array.add(nestedArray);

    outputMustBe("[[1]]");
}
*/