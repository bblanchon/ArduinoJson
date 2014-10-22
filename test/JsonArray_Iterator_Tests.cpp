#include <gtest/gtest.h>
#include <ArduinoJson/JsonArray.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(JsonArray_Iterator_Test, SimpleTest)
{
	StaticJsonBuffer<42> jsonBuffer;

	JsonArray array = jsonBuffer.createArray();
	array.add(12);
	array.add(34);

	JsonArrayIterator it = array.begin();
	JsonArrayIterator end = array.end();

	EXPECT_NE(end, it);
	EXPECT_EQ(12, static_cast<int>(*it));
	++it;
	EXPECT_NE(end, it);
	EXPECT_EQ(34, static_cast<int>(*it));
	++it;
	EXPECT_EQ(array.end(), it);
}