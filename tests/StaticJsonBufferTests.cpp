#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>

TEST(StaticJsonBuffer, CapacityMatchTemplateParameter)
{
    StaticJsonBuffer<42> json;   
    EXPECT_EQ(42, json.capacity());
}

TEST(StaticJsonBuffer, InitialSizeIsZero)
{
    StaticJsonBuffer<42> json;
    EXPECT_EQ(0, json.size());
}

TEST(StaticJsonBuffer, WhenCreateObjectIsCalled_ThenSizeIsIncreasedByOne)
{
    StaticJsonBuffer<42> json;

    json.createObject();
    EXPECT_EQ(1, json.size());
    
    json.createObject();
    EXPECT_EQ(2, json.size());
}

TEST(StaticJsonBuffer, GivenBufferIsFull_WhenCreateObjectIsCalled_ThenSizeDoesNotChange)
{
    StaticJsonBuffer<1> json;

    json.createObject();
    EXPECT_EQ(1, json.size());

    json.createObject();
    EXPECT_EQ(1, json.size());
}