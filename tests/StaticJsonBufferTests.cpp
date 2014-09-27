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