#include <gtest/gtest.h>
#include <JsonObject.h>
#include <StaticJsonBuffer.h>

class JsonObjectSerializationTests : public testing::Test
{
protected:
    virtual void SetUp()
    {
        object = json.createObject();
    }

    void jsonMustBe(const char* expected)
    {        
        char actual[256];
        int result = object.printTo(actual, sizeof(actual));

        EXPECT_STREQ(expected, actual);
        EXPECT_EQ(strlen(expected), result);
    }

    JsonObject object;

private:
    StaticJsonBuffer<42> json;
};

TEST_F(JsonObjectSerializationTests, EmptyObject)
{
    jsonMustBe("{}");
}