Migrating code written for Arduino JSON v3 to v4
================================================

Arduino JSON v4 was a major rewrite of the library, and the API changed significantly.

## Includes

Arduino JSON v3 had two include files:

    #include <JsonParser.h>
    #include <JsonGenerator.h>

Arduino JSON v4 only has one:

	#include <ArduinoJson.h>

## Namespaces

Arduino JSON v3 had two namespaces:

	using namespace ArduinoJson::Parser;
	using namespace ArduinoJson::Generator;

Arduino JSON v4 doesn't require the `using namespace` statement.
It has a namespace but the `using namespace` is done in the header file.

## StaticJsonBuffer

Arduino JSON v3 had different memory allocation models for the parser:

	JsonParser<16> parser; // 16 being the capacity in "tokens"

and for the generator:

	JsonArray<4> array; // 4 being the number of element
	JsonObject<4> object;

Arduino JSON v4 only has one memory allocation model:

	StaticJsonBuffer<128> buffer; // 128 being the capacity in bytes

## Return values for the parser

Arduino JSON v3 returned value types:

	JsonArray array = parser.parseArray(json);
	JsonObject object = parser.parseObject(json);

Arduino JSON v4 returns references types:

	JsonArray& array = buffer.parseArray(json);
	JsonObject& object = buffer.parseObject(json);

Everything else is compatible

## Creating arrays and objects

Arduino JSON v3 allowed to create `JsonArray` and `JsonObject` directly:

	JsonArray<4> array;
	JsonObject<4> object;

Arduino JSON v4 requires that you use a `StaticJsonBuffer` for that:

	JsonArray& array = buffer.createArray();
	JsonObject& object = buffer.createObject();

Note: you don't have to specify the capacity anymore.

## Printable interface

Arduino JSON v3 used to implement the Printable interface, which allowed statements like:

    Serial.print(array);

But Arduino JSON v4 doesn't, instead you need to write this:

	array.printTo(Serial);

Note: there is a good reason for that: reducing the size of `JsonArray` and `JsonObject`.