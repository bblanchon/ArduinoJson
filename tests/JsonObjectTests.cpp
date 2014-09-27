#include <gtest/gtest.h>
#include <StaticJsonBuffer.h>
#include <JsonValue.h>

TEST(JsonObjectTests, WhenValueIsAdded_ThenSizeIsIncreasedByOne)
{
    StaticJsonBuffer<42> json;

    JsonObject object = json.createObject();

    object["hello"];
    EXPECT_EQ(1, object.size());

    object["world"];
    EXPECT_EQ(2, object.size());
}