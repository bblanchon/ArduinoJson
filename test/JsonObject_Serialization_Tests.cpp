#include <gtest/gtest.h>
#include <ArduinoJson/JsonArray.h>
#include <ArduinoJson/JsonObject.h>
#include <ArduinoJson/JsonValue.h>
#include <ArduinoJson/StaticJsonBuffer.h>

using namespace ArduinoJson;

class JsonObject_Serialization_Tests : public testing::Test
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
    StaticJsonBuffer<5> json;
};

TEST_F(JsonObject_Serialization_Tests, EmptyObject)
{
    outputMustBe("{}");
}

TEST_F(JsonObject_Serialization_Tests, OneString)
{
    object["key"] = "value";

    outputMustBe("{\"key\":\"value\"}");
}

TEST_F(JsonObject_Serialization_Tests, TwoStrings)
{
    object["key1"] = "value1";
    object["key2"] = "value2";

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_Serialization_Tests, RemoveFirst)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key1");

    outputMustBe("{\"key2\":\"value2\"}");
}

TEST_F(JsonObject_Serialization_Tests, RemoveLast)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key2");

    outputMustBe("{\"key1\":\"value1\"}");
}

TEST_F(JsonObject_Serialization_Tests, RemoveUnexistingKey)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object.remove("key3");

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_Serialization_Tests, ReplaceExistingKey)
{
    object["key"] = "value1";
    object["key"] = "value2";

    outputMustBe("{\"key\":\"value2\"}");
}

TEST_F(JsonObject_Serialization_Tests, OneStringOverCapacity)
{
    object["key1"] = "value1";
    object["key2"] = "value2";
    object["key3"] = "value3";

    outputMustBe("{\"key1\":\"value1\",\"key2\":\"value2\"}");
}

TEST_F(JsonObject_Serialization_Tests, OneInteger)
{
    object["key"] = 1;
    outputMustBe("{\"key\":1}");
}

TEST_F(JsonObject_Serialization_Tests, OneDoubleFourDigits)
{
    object["key"].set(3.14159265358979323846, 4);
    outputMustBe("{\"key\":3.1416}");
}

TEST_F(JsonObject_Serialization_Tests, OneDoubleDefaultDigits)
{
    object["key"] = 3.14159265358979323846;
    outputMustBe("{\"key\":3.14}");
}

TEST_F(JsonObject_Serialization_Tests, OneNull)
{
    object["key"] = (char*) 0;
    outputMustBe("{\"key\":null}");
}

TEST_F(JsonObject_Serialization_Tests, OneTrue)
{
    object["key"] = true;
    outputMustBe("{\"key\":true}");
}

TEST_F(JsonObject_Serialization_Tests, OneFalse)
{
    object["key"] = false;
    outputMustBe("{\"key\":false}");
}

TEST_F(JsonObject_Serialization_Tests, OneEmptyNestedArrayViaProxy)
{
    JsonArray nestedArray = json.createArray();

    object["key"] = nestedArray;

    outputMustBe("{\"key\":[]}");
}

TEST_F(JsonObject_Serialization_Tests, OneEmptyNestedObjectViaProxy)
{
    JsonObject nestedArray = json.createObject();

    object["key"] = nestedArray;

    outputMustBe("{\"key\":{}}");
}

TEST_F(JsonObject_Serialization_Tests, OneEmptyNestedObject)
{
    object.createNestedObject("key");

    outputMustBe("{\"key\":{}}");
}

TEST_F(JsonObject_Serialization_Tests, OneEmptyNestedArray)
{
    object.createNestedArray("key");

    outputMustBe("{\"key\":[]}");
}