#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

class JsonArray_Container_Tests : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        array = json.createArray();
    }

    StaticJsonBuffer<42> json;
    JsonArray array;
};

TEST_F(JsonArray_Container_Tests, InitialSizeIsZero)
{
    EXPECT_EQ(0, array.size());
}

TEST_F(JsonArray_Container_Tests, Grow_WhenValuesAreAdded)
{
    array.add("hello");
    EXPECT_EQ(1, array.size());

    array.add("world");
    EXPECT_EQ(2, array.size());
}

TEST_F(JsonArray_Container_Tests, CanStoreIntegers)
{
    array.add(123);
    array.add(456);

    EXPECT_EQ(123, (int) array[0]);
    EXPECT_EQ(456, (int) array[1]);
}

TEST_F(JsonArray_Container_Tests, CanStoreDoubles)
{
    array.add(123.45);
    array.add(456.78);

    EXPECT_EQ(123.45, (double) array[0]);
    EXPECT_EQ(456.78, (double) array[1]);
}
/*
TEST_F(JsonArray_Container_Tests, CanStoreBooleans)
{
    array.add(true);
    array.add(false);

    EXPECT_TRUE((bool) array[0]);
    EXPECT_FALSE((bool) array[1]);
}

TEST_F(JsonArray_Container_Tests, CanStoreStrings)
{
    array.add("h3110");
    array.add("w0r1d");

    EXPECT_STREQ("h3110", (const char*) array[0]);
    EXPECT_STREQ("w0r1d", (const char*) array[1]);
}

TEST_F(JsonArray_Container_Tests, CanStoreInnerArrays)
{
    JsonArray innerarray1 = json.createArray();
    JsonArray innerarray2 = json.createArray();

    array.add(innerarray1);
    array.add(innerarray2);

    EXPECT_EQ(innerarray1, (JsonArray) array[0]);
    EXPECT_EQ(innerarray2, (JsonArray) array[1]);
}*/