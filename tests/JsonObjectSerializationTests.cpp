#include <gtest/gtest.h>
#include <JsonObject.h>
#include <JsonValue.h>
#include <StaticJsonBuffer.h>

class JsonObjectSerializationTests : public testing::Test
{
protected:
    virtual void SetUp()
    {
        object = json.createObject();
    }

    void outputMustBe(const char* expected)
    {        
        char actual[256];
        int result = object.printTo(actual, sizeof(actual));

        EXPECT_STREQ(expected, actual);
        EXPECT_EQ(strlen(expected), result);
    }

    JsonObject object;

private:
    StaticJsonBuffer<5> json;
};

TEST_F(JsonObjectSerializationTests, EmptyObject)
{
    outputMustBe("{}");
}

TEST_F(JsonObjectSerializationTests, OneString)
{
    object["key"] = "value";

    outputMustBe("{\"key\":\"value\"}");
}

TEST_F(JsonObjectSerializationTests, TwoStrings)
{
    object["key1"] = "value1";
    object["key2"] = "value2";

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObjectSerializationTests, RemoveFirst)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key1");

    outputMustBe("{\"key2\":\"value2\"}");
}

TEST_F(JsonObjectSerializationTests, RemoveLast)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key2");

    outputMustBe("{\"key1\":\"value1\"}");
}

TEST_F(JsonObjectSerializationTests, RemoveUnexistingKey)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key3");

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObjectSerializationTests, ReplaceExistingKey)
{
    object["key"] = "value1";
    object["key"] = "value2";

    outputMustBe("{\"key\":\"value2\"}");
}

TEST_F(JsonObjectSerializationTests, OneStringOverCapacity)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object["key3"] = "value3";

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObjectSerializationTests, OneInteger)
{
    object["key"] = 1;
    outputMustBe("{\"key\":1}");
}

TEST_F(JsonObjectSerializationTests, OneDoubleFourDigits)
{
    object["key"].set(3.14159265358979323846, 4);
    outputMustBe("{\"key\":3.1416}");
}

TEST_F(JsonObjectSerializationTests, OneDoubleDefaultDigits)
{
    object["key"] = 3.14159265358979323846;
    outputMustBe("{\"key\":3.14}");
}
/*
TEST_F(JsonObjectSerializationTests, OneNull)
{
    object["key"] = (char*) 0;
    outputMustBe("{\"key\":null}");
}

TEST_F(JsonObjectSerializationTests, OneTrue)
{
    object["key"] = true;
    outputMustBe("{\"key\":true}");
}

TEST_F(JsonObjectSerializationTests, OneFalse)
{
    object["key"] = false;
    outputMustBe("{\"key\":false}");
}

TEST_F(JsonObjectSerializationTests, OneEmptyNestedArray)
{
    auto nestedArray = JsonArray<1>();

    object["key"] = nestedArray;

    outputMustBe("{\"key\":[]}");
}

TEST_F(JsonObjectSerializationTests, OneEmptyNestedObject)
{
    auto nestedObject = JsonObject<1>();

    object["key"] = nestedObject;

    outputMustBe("{\"key\":{}}");
}*/