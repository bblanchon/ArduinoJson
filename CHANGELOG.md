ArduinoJson: change log
=======================

v5.6.2
------

* Fixed build when another lib does `#undef isnan` (issue #284)

v5.6.1
------

* Added missing `#pragma once` (issue #310)

v5.6.0
------

* ArduinoJson is now a header-only library (issue #199)

v5.5.1
------

* Fixed compilation error with Intel Galileo (issue #299)

v5.5.0
------

* Added `JsonVariant::success()` (issue #279)
* Renamed `JsonVariant::invalid<T>()` to `JsonVariant::defaultValue<T>()`

v5.4.0
------

* Changed `::String` to `ArduinoJson::String` (issue #275)
* Changed `::Print` to `ArduinoJson::Print` too

v5.3.0
------

* Added custom implementation of `ftoa` (issues #266, #267, #269 and #270)
* Added `JsonVariant JsonBuffer::parse()` (issue #265)
* Fixed `unsigned long` printed as `signed long` (issue #170)

v5.2.0
------

* Added `JsonVariant::as<char*>()` as a synonym for `JsonVariant::as<const char*>()` (issue #257)
* Added example `JsonHttpClient` (issue #256)
* Added `JsonArray::copyTo()` and `JsonArray::copyFrom()` (issue #254)
* Added `RawJson()` to insert pregenerated JSON portions (issue #259)

v5.1.1
------

* Removed `String` duplication when one replaces a value in a `JsonObject` (PR #232 by @ulion)

v5.1.0
------

* Added support of `long long` (issue #171)
* Moved all build settings to `ArduinoJson/Configuration.hpp`

**BREAKING CHANGE**:
If you defined `ARDUINOJSON_ENABLE_STD_STREAM`, you now need to define it to `1`.

v5.0.8
------

* Made the library compatible with [PlatformIO](http://platformio.org/) (issue #181)
* Fixed `JsonVariant::is<bool>()` that was incorrectly returning false (issue #214)

v5.0.7
------

* Made library easier to use from a CMake project: simply `add_subdirectory(ArduinoJson/src)`
* Changed `String` to be a `typedef` of `std::string` (issues #142 and #161)

**BREAKING CHANGES**:
- `JsonVariant(true).as<String>()` now returns `"true"` instead of `"1"`
- `JsonVariant(false).as<String>()` now returns `"false"` instead of `"0"`

v5.0.6
------

* Added parameter to `DynamicJsonBuffer` constructor to set initial size (issue #152)
* Fixed warning about library category in Arduino 1.6.6 (issue #147)
* Examples: Added a loop to wait for serial port to be ready (issue #156)

v5.0.5
------

* Added overload `JsonObjectSuscript::set(value, decimals)` (issue #143)
* Use `float` instead of `double` to reduce the size of `JsonVariant` (issue #134)

v5.0.4
------

* Fixed ambiguous overload with `JsonArraySubscript` and `JsonObjectSubscript` (issue #122)

v5.0.3
------

* Fixed `printTo(String)` which wrote numbers instead of strings (issue #120)
* Fixed return type of `JsonArray::is<T>()` and some others (issue #121)

v5.0.2
------

* Fixed segmentation fault in `parseObject(String)` and `parseArray(String)`, when the 
  `StaticJsonBuffer` is too small to hold a copy of the string
* Fixed Clang warning "register specifier is deprecated" (issue #102)
* Fixed GCC warning "declaration shadows a member" (issue #103)
* Fixed memory alignment, which made ESP8266 crash (issue #104)
* Fixed compilation on Visual Studio 2010 and 2012 (issue #107)

v5.0.1
------

* Fixed compilation with Arduino 1.0.6 (issue #99)

v5.0.0
------

* Added support of `String` class (issues #55, #56, #70, #77)
* Added `JsonBuffer::strdup()` to make a copy of a string (issues #10, #57)
* Implicitly call `strdup()` for `String` but not for `char*` (issues #84, #87)
* Added support of non standard JSON input (issue #44)
* Added support of comments in JSON input (issue #88)
* Added implicit cast between numerical types (issues #64, #69, #93)
* Added ability to read number values as string (issue #90)
* Redesigned `JsonVariant` to leverage converting constructors instead of assignment operators (issue #66)
* Switched to new the library layout (requires Arduino 1.0.6 or above)

**BREAKING CHANGES**:
- `JsonObject::add()` was renamed to `set()`
- `JsonArray::at()` and `JsonObject::at()` were renamed to `get()`
- Number of digits of floating point value are now set with `double_with_n_digits()`

**Personal note about the `String` class**:
Support of the `String` class has been added to the library because many people use it in their programs.
However, you should not see this as an invitation to use the `String` class.
The `String` class is **bad** because it uses dynamic memory allocation.
Compared to static allocation, it compiles to a bigger, slower program, and is less predictable.
You certainly don't want that in an embedded environment!

v4.6
----

* Fixed segmentation fault in `DynamicJsonBuffer` when memory allocation fails (issue #92)

v4.5
----

* Fixed buffer overflow when input contains a backslash followed by a terminator (issue #81)

**Upgrading is recommended** since previous versions contain a potential security risk.

Special thanks to [Giancarlo Canales Barreto](https://github.com/gcanalesb) for finding this nasty bug.

v4.4
----

* Added `JsonArray::measureLength()` and `JsonObject::measureLength()` (issue #75)

v4.3
----

* Added `JsonArray::removeAt()` to remove an element of an array (issue #58)
* Fixed stack-overflow in `DynamicJsonBuffer` when parsing huge JSON files (issue #65)
* Fixed wrong return value of `parseArray()` and `parseObject()` when allocation fails (issue #68)

v4.2
----

* Switched back to old library layout (issues #39, #43 and #45)
* Removed global new operator overload (issue #40, #45 and #46)
* Added an example with EthernetServer

v4.1
----

* Added DynamicJsonBuffer (issue #19)

v4.0
----

* Unified parser and generator API (issue #23)
* Updated library layout, now requires Arduino 1.0.6 or newer

**BREAKING CHANGE**: API changed significantly, see [Migrating code to the new API](https://github.com/bblanchon/ArduinoJson/wiki/Migrating-code-to-the-new-API).


v3.4
----

* Fixed escaped char parsing (issue #16)


v3.3
----

* Added indented output for the JSON generator (issue #11), see example bellow.
* Added `IndentedPrint`, a decorator for `Print` to allow indented output

Example:

    JsonOject<2> json;
    json["key"] = "value";
    json.prettyPrintTo(Serial);

v3.2
----

* Fixed a bug when adding nested object in `JsonArray` (bug introduced in v3.1).

v3.1
----

* Calling `Generator::JsonObject::add()` twice with the same `key` now replaces the `value`
* Added `Generator::JsonObject::operator[]`, see bellow the new API
* Added `Generator::JsonObject::remove()` (issue #9)

Old generator API:

	JsonObject<3> root; 
    root.add("sensor", "gps");
    root.add("time", 1351824120);
    root.add("data", array);

New generator API:

	JsonObject<3> root; 
    root["sensor"] = "gps";
    root["time"] = 1351824120;
    root["data"] = array;

v3.0
----

* New parser API, see bellow
* Renamed `JsonHashTable` into `JsonObject`
* Added iterators for `JsonArray` and `JsonObject` (issue #4)

Old parser API:

    JsonHashTable root = parser.parseHashTable(json);

	char*  sensor    = root.getString("sensor");
	long   time      = root.getLong("time");
	double latitude  = root.getArray("data").getDouble(0);
    double longitude = root.getArray("data").getDouble(1);

New parser API:

	JsonObject root = parser.parse(json);

	char*  sensor    = root["sensor"];
    long   time      = root["time"];
    double latitude  = root["data"][0];
    double longitude = root["data"][1];

v2.1
----

* Fixed case `#include "jsmn.cpp"` which caused an error in Linux (issue #6)
* Fixed a buffer overrun in JSON Parser (issue #5)

v2.0
----

* Added JSON encoding (issue #2)
* Renamed the library `ArduinoJsonParser` becomes `ArduinoJson`

**Breaking change**: you need to add the following line at the top of your program.

	using namespace ArduinoJson::Parser;

v1.2
----

* Fixed error in JSON parser example (issue #1)

v1.1
----

* Example: changed `char* json` into `char[] json` so that the bytes are not write protected
* Fixed parsing bug when the JSON contains multi-dimensional arrays

v1.0 
----

Initial release
