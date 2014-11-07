Migrating code written for Arduino JSON v3 to v4
================================================

Arduino JSON v4 was a major rewrite of the library, and the API change significantly.

## Includes

Arduino JSON v3 had two include files:

    #include <JsonParser.h>
    #include <JsonGenerator.h>

Arduino JSON v4 only has one:

	#include <ArduinoJson.h>

Node: the header `src/ArduinoJson.h` is intended to be used within the Arduino IDE, if you're in another environment, you may need to include the following headers:

	#include <ArduinoJson/StaticJsonBuffer.hpp>
	#include <ArduinoJson/JsonObject.hpp>
	#include <ArduinoJson/JsonArray.hpp>

## Namespaces

Arduino JSON v3 had two namespaces:

	using namespace ArduinoJson::Parser;
	using namespace ArduinoJson::Generator;

Arduino JSON v4 only has one:

	using namespace ArduinoJson;

If you include the header `ArduinoJson.h` (recommended if in Arduino IDE), the `using` directivei is already done for you, so you don't have to write it.

## StaticJsonBuffer

Arduino JSON v3 had different memory allocation models for parser:

	JsonParser<16> parser; // 16 being the capacity in "tokens"

and for the generator:

	JsonArray<4> array; // 4 being the number of element
	JsonObject<4> object;

Arduino JSON v4 only has one memory allocation mode:

	StaticJsonBuffer<128> buffer; // 128 being the capacity in bytes

## Return values for the parser

Arduino JSON v3 returned `JsonArray` and `JsonObject`:

	JsonArray array = parser.parseArray(json);
	JsonObject object = parser.parseObject(json);

Arduino JSON v4 returns references:

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

Arduino JSON v3 used to implement the Printable interface, that allowed that kind of statement:

    Serial.print(array);

Arduino JSON v4 doesn't, so you need to write this:

	array.printTo(Serial);

Note: there is a good reason for that: reducing the size of `JsonArray` and `JsonObject`.