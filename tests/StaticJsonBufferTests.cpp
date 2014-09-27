#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>

TEST(StaticJsonBufferTests, CapacityMatchTemplateParameter)
{
    StaticJsonBuffer<42> json;   
    EXPECT_EQ(42, json.capacity());
}