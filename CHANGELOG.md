ArduinoJson: change log
=======================

v6.17.1 (2020-11-07)
-------

* Fixed error `ambiguous overload for 'operator|'` (issue #1411)
* Fixed `operator|(MemberProxy, JsonObject)` (issue #1415)
* Allowed more than 32767 values in non-embedded mode (issue #1414)

v6.17.0 (2020-10-19)
-------

* Added a build failure when nullptr is defined as a macro (issue #1355)
* Added `JsonDocument::overflowed()` which tells if the memory pool was too small (issue #1358)
* Added `DeserializationError::EmptyInput` which tells if the input was empty
* Added `DeserializationError::f_str()` which returns a `const __FlashStringHelper*` (issue #846)
* Added `operator|(JsonVariantConst, JsonVariantConst)`
* Added filtering for MessagePack (issue #1298, PR #1394 by Luca Passarella)
* Moved float convertion tables to PROGMEM
* Fixed `JsonVariant::set((char*)0)` which returned false instead of true (issue #1368)
* Fixed error `No such file or directory #include <WString.h>` (issue #1381)

v6.16.1 (2020-08-04)
-------

* Fixed `deserializeJson()` that stopped reading after `{}` (issue #1335)

v6.16.0 (2020-08-01)
-------

* Added comparisons (`>`, `>=`, `==`, `!=`, `<`, and `<=`) between `JsonVariant`s
* Added string deduplication (issue #1303)
* Added `JsonString::operator!=`
* Added wildcard key (`*`) for filters (issue #1309)
* Set `ARDUINOJSON_DECODE_UNICODE` to `1` by default
* Fixed `copyArray()` not working with `String`, `ElementProxy`, and `MemberProxy`
* Fixed error `getOrAddElement is not a member of ElementProxy` (issue #1311)
* Fixed excessive stack usage when compiled with `-Og` (issues #1210 and #1314)
* Fixed `Warning[Pa093]: implicit conversion from floating point to integer` on IAR compiler (PR #1328 by @stawiski)

v6.15.2 (2020-05-15)
-------

* CMake: don't build tests when imported in another project
* CMake: made project arch-independent
* Visual Studio: fixed error C2766 with flag `/Zc:__cplusplus` (issue #1250)
* Added support for `JsonDocument` to `copyArray()` (issue #1255)
* Added support for `enum`s in `as<T>()` and `is<T>()`  (issue #1256)
* Added `JsonVariant` as an input type for `deserializeXxx()`  
  For example, you can do: `deserializeJson(doc2, doc1["payload"])`
* Break the build if using 64-bit integers with ARDUINOJSON_USE_LONG_LONG==0

v6.15.1 (2020-04-08)
-------

* Fixed "maybe-uninitialized" warning (issue #1217)
* Fixed "statement is unreachable" warning on IAR (issue #1233)
* Fixed "pointless integer comparison" warning on IAR (issue #1233)
* Added CMake "install" target (issue #1209)
* Disabled alignment on AVR (issue #1231)

v6.15.0 (2020-03-22)
-------

* Added `DeserializationOption::Filter` (issue #959)
* Added example `JsonFilterExample.ino`
* Changed the array subscript operator to automatically add missing elements
* Fixed "deprecated-copy" warning on GCC 9 (fixes #1184)
* Fixed `MemberProxy::set(char[])` not duplicating the string (issue #1191)
* Fixed enums serialized as booleans (issue #1197)
* Fixed incorrect string comparison on some platforms (issue #1198)
* Added move-constructor and move-assignment to `BasicJsonDocument`
* Added `BasicJsonDocument::garbageCollect()` (issue #1195)
* Added `StaticJsonDocument::garbageCollect()`
* Changed copy-constructor of `BasicJsonDocument` to preserve the capacity of the source.
* Removed copy-constructor of `JsonDocument` (issue #1189)

> ### BREAKING CHANGES
> 
> #### Copy-constructor of `BasicJsonDocument`
>
> In previous versions, the copy constructor of `BasicJsonDocument` looked at the source's `memoryUsage()` to choose its capacity.
> Now, the copy constructor of `BasicJsonDocument` uses the same capacity as the source.
>
> Example:
>
> ```c++
> DynamicJsonDocument doc1(64);
> doc1.set(String("example"));
>
> DynamicJsonDocument doc2 = doc1;
> Serial.print(doc2.capacity());  // 8 with ArduinoJson 6.14
>                                 // 64 with ArduinoJson 6.15
> ```
>
> I made this change to get consistent results between copy-constructor and move-constructor, and whether RVO applies or not.
>
> If you use the copy-constructor to optimize your documents, you can use `garbageCollect()` or `shrinkToFit()` instead.
>
> #### Copy-constructor of `JsonDocument`
>
> In previous versions, it was possible to create a function that take a `JsonDocument` by value.
>
> ```c++
> void myFunction(JsonDocument doc) {}
> ```
>
> This function gives the wrong clues because it doesn't receive a copy of the `JsonDocument`, only a sliced version.
> It worked because the copy constructor copied the internal pointers, but it was an accident.
>
> From now, if you need to pass a `JsonDocument` to a function, you must use a reference:
>
> ```c++
> void myFunction(JsonDocument& doc) {}
> ```

v6.14.1 (2020-01-27)
-------

* Fixed regression in UTF16 decoding (issue #1173)
* Fixed `containsKey()` on `JsonVariantConst`
* Added `getElement()` and `getMember()` to `JsonVariantConst`

v6.14.0 (2020-01-16)
-------

* Added `BasicJsonDocument::shrinkToFit()`
* Added support of `uint8_t` for `serializeJson()`, `serializeJsonPretty()`, and `serializeMsgPack()` (issue #1142)
* Added `ARDUINOJSON_ENABLE_COMMENTS` to enable support for comments (defaults to 0)
* Auto enable support for `std::string` and `std::stream` on modern compilers (issue #1156)
  (No need to define `ARDUINOJSON_ENABLE_STD_STRING` and `ARDUINOJSON_ENABLE_STD_STREAM` anymore)
* Improved decoding of UTF-16 surrogate pairs (PR #1157 by @kaysievers)
  (ArduinoJson now produces standard UTF-8 instead of CESU-8)
* Added `measureJson`, `measureJsonPretty`, and `measureMsgPack` to `keywords.txt`
  (This file is used for syntax highlighting in the Arduino IDE) 
* Fixed `variant.is<nullptr_t>()`
* Fixed value returned by `serializeJson()`, `serializeJsonPretty()`, and `serializeMsgPack()` when writing to a `String`
* Improved speed of `serializeJson()`, `serializeJsonPretty()`, and `serializeMsgPack()` when writing to a `String`

> ### BREAKING CHANGES
> 
> #### Comments
> 
> Support for comments in input is now optional and disabled by default.
>
> If you need support for comments, you must defined `ARDUINOJSON_ENABLE_COMMENTS` to `1`; otherwise, you'll receive `InvalidInput` errors.
>
> ```c++
> #define ARDUINOJSON_ENABLE_COMMENTS 1
> #include <ArduinoJson.h>
> ```

v6.13.0 (2019-11-01)
-------

* Added support for custom writer/reader classes (issue #1088)
* Added conversion from `JsonArray` and `JsonObject` to `bool`, to be consistent with `JsonVariant`
* Fixed `deserializeJson()` when input contains duplicate keys (issue #1095)
* Improved `deserializeMsgPack()` speed by reading several bytes at once
* Added detection of Atmel AVR8/GNU C Compiler (issue #1112)
* Fixed deserializer that stopped reading at the first `0xFF` (PR #1118 by @mikee47)
* Fixed dangling reference in copies of `MemberProxy` and `ElementProxy` (issue #1120)

v6.12.0 (2019-09-05)
-------

* Use absolute instead of relative includes (issue #1072)
* Changed `JsonVariant::as<bool>()` to return `true` for any non-null value (issue #1005)
* Moved ancillary files to `extras/` (issue #1011)

v6.11.5 (2019-08-23)
-------

* Added fallback implementations of `strlen_P()`, `strncmp_P()`, `strcmp_P()`, and `memcpy_P()` (issue #1073)

v6.11.4 (2019-08-12)
-------

* Added `measureJson()` to the `ArduinoJson` namespace (PR #1069 by @nomis)
* Added support for `basic_string<char, traits, allocator>` (issue #1045)
* Fixed example `JsonConfigFile.ino` for ESP8266
* Include `Arduino.h` if `ARDUINO` is defined (PR #1071 by @nomis)

v6.11.3 (2019-07-22)
-------

* Added operators `==` and `!=` for `JsonDocument`, `ElementProxy`, and `MemberProxy`
* Fixed comparison of `JsonVariant` when one contains a linked string and the other contains an owned string (issue #1051)

v6.11.2 (2019-07-08)
-------

* Fixed assignment of `JsonDocument` to `JsonVariant` (issue #1023)
* Fix invalid conversion error on Particle Argon (issue #1035)

v6.11.1 (2019-06-21)
-------

* Fixed `serialized()` not working with Flash strings (issue #1030)

v6.11.0 (2019-05-26)
-------

* Fixed `deserializeJson()` silently accepting a `Stream*` (issue #978)
* Fixed invalid result from `operator|` (issue #981)
* Made `deserializeJson()` more picky about trailing characters (issue #980)
* Added `ARDUINOJSON_ENABLE_NAN` (default=0) to enable NaN in JSON (issue #973)
* Added `ARDUINOJSON_ENABLE_INFINITY` (default=0) to enable Infinity in JSON
* Removed implicit conversion in comparison operators (issue #998)
* Added lexicographical comparison for `JsonVariant`
* Added support for `nullptr` (issue #998)

> ### BREAKING CHANGES
> 
> #### NaN and Infinity
> 
> The JSON specification allows neither NaN not Infinity, but previous
> versions of ArduinoJson supported it. Now, ArduinoJson behaves like most
> other libraries: a NaN or and Infinity in the `JsonDocument`, becomes
> a `null` in the output JSON. Also, `deserializeJson()` returns
> `InvalidInput` if the JSON document contains NaN or Infinity.
> 
> This version still supports NaN and Infinity in JSON documents, but
> it's disabled by default to be compatible with other JSON parsers.
> If you need the old behavior back, define `ARDUINOJSON_ENABLE_NAN` and
> `ARDUINOJSON_ENABLE_INFINITY` to `1`;:
> 
> ```c++
> #define ARDUINOJSON_ENABLE_NAN 1
> #define ARDUINOJSON_ENABLE_INFINITY 1
> #include <ArduinoJson.h>
> ```
> 
> #### The "or" operator
> 
> This version slightly changes the behavior of the | operator when the 
> variant contains a float and the user requests an integer.
>
> Older versions returned the floating point value truncated.
> Now, it returns the default value.
> 
> ```c++
> // suppose variant contains 1.2
> int value = variant | 3;
> 
> // old behavior:
> value == 1
> 
> // new behavior
> value == 3
> ```
> 
> If you need the old behavior, you must add `if (variant.is<float>())`.

v6.10.1 (2019-04-23)
-------

* Fixed error "attributes are not allowed on a function-definition"
* Fixed `deserializeJson()` not being picky enough (issue #969)
* Fixed error "no matching function for call to write(uint8_t)" (issue #972)

v6.10.0 (2019-03-22)
-------

* Fixed an integer overflow in the JSON deserializer
* Added overflow handling in `JsonVariant::as<T>()` and `JsonVariant::is<T>()`.
   - `as<T>()` returns `0` if the integer `T` overflows
   - `is<T>()` returns `false` if the integer `T` overflows
* Added `BasicJsonDocument` to support custom allocator (issue #876)
* Added `JsonDocument::containsKey()` (issue #938)
* Added `JsonVariant::containsKey()`

v6.9.1 (2019-03-01)
------

* Fixed warning "unused variable" with GCC 4.4 (issue #912)
* Fixed warning "cast  increases required alignment" (issue #914)
* Fixed warning "conversion may alter value" (issue #914)
* Fixed naming conflict with "CAPACITY" (issue #839)
* Muted warning "will change in GCC 7.1" (issue #914)
* Added a clear error message for `StaticJsonBuffer` and `DynamicJsonBuffer`
* Marked ArduinoJson.h  as a "system header"

v6.9.0 (2019-02-26)
------

* Decode escaped Unicode characters like \u00DE (issue #304, PR #791)
  Many thanks to Daniel Schulte (aka @trilader) who implemented this feature.
* Added option ARDUINOJSON_DECODE_UNICODE to enable it
* Converted `JsonArray::copyFrom()/copyTo()` to free functions `copyArray()`
* Renamed `JsonArray::copyFrom()` and `JsonObject::copyFrom()` to `set()`
* Renamed `JsonArray::get()` to `getElement()`
* Renamed `JsonArray::add()` (without arg) to `addElement()`
* Renamed `JsonObject::get()` to `getMember()`
* Renamed `JsonObject::getOrCreate()` to `getOrAddMember()`
* Fixed `JsonVariant::isNull()` not returning `true` after `set((char*)0)`
* Fixed segfault after `variant.set(serialized((char*)0))`
* Detect `IncompleteInput` in `false`, `true`, and `null`
* Added `JsonDocument::size()`
* Added `JsonDocument::remove()`
* Added `JsonVariant::clear()`
* Added `JsonVariant::remove()`

v6.8.0-beta (2019-01-30)
-----------

* Import functions in the ArduinoJson namespace to get clearer errors
* Improved syntax highlighting in Arduino IDE
* Removed default capacity of `DynamicJsonDocument`
* `JsonArray::copyFrom()` accepts `JsonArrayConst`
* `JsonVariant::set()` accepts `JsonArrayConst` and `JsonObjectConst`
* `JsonDocument` was missing in the ArduinoJson namespace
* Added `memoryUsage()` to `JsonArray`, `JsonObject`, and `JsonVariant`
* Added `nesting()` to `JsonArray`, `JsonDocument`, `JsonObject`, and `JsonVariant`
* Replaced `JsonDocument::nestingLimit` with an additional parameter
  to `deserializeJson()` and `deserializeMsgPack()`
* Fixed uninitialized variant in `JsonDocument`
* Fixed `StaticJsonDocument` copy constructor and copy assignment
* The copy constructor of `DynamicJsonDocument` chooses the capacity according to the memory usage of the source, not from the capacity of the source.
* Added the ability to create/assign a `StaticJsonDocument`/`DynamicJsonDocument` from a `JsonArray`/`JsonObject`/`JsonVariant`
* Added `JsonDocument::isNull()`
* Added `JsonDocument::operator[]`
* Added `ARDUINOJSON_TAB` to configure the indentation character
* Reduced the size of the pretty JSON serializer
* Added `add()`, `createNestedArray()` and `createNestedObject()` to `JsonVariant`
* `JsonVariant` automatically promotes to `JsonObject` or `JsonArray` on write.
  Calling `JsonVariant::to<T>()` is not required anymore.
* `JsonDocument` now support the same operations as `JsonVariant`.
  Calling `JsonDocument::as<T>()` is not required anymore.
* Fixed example `JsonHttpClient.ino`
* User can now use a `JsonString` as a key or a value

> ### BREAKING CHANGES
> 
> #### `DynamicJsonDocument`'s constructor
> 
> The parameter to the constructor of `DynamicJsonDocument` is now mandatory
>
> Old code:
>
> ```c++
> DynamicJsonDocument doc;
> ```
>
> New code:
>
> ```c++
> DynamicJsonDocument doc(1024);
> ```
> 
> #### Nesting limit
> 
> `JsonDocument::nestingLimit` was replaced with a new parameter to `deserializeJson()` and `deserializeMsgPack()`.
> 
> Old code:
> 
> ```c++
> doc.nestingLimit = 15;
> deserializeJson(doc, input);
> ```
> 
> New code: 
> 
> ```c++
> deserializeJson(doc, input, DeserializationOption::NestingLimit(15));
> ```

v6.7.0-beta (2018-12-07)
-----------

* Removed the automatic expansion of `DynamicJsonDocument`, it now has a fixed capacity.
* Restored the monotonic allocator because the code was getting too big
* Reduced the memory usage
* Reduced the code size
* Renamed `JsonKey` to `JsonString`
* Removed spurious files in the Particle library

v6.6.0-beta (2018-11-13)
-----------

* Removed `JsonArray::is<T>(i)` and `JsonArray::set(i,v)`
* Removed `JsonObject::is<T>(k)` and `JsonObject::set(k,v)`
* Replaced `T JsonArray::get<T>(i)` with `JsonVariant JsonArray::get(i)`
* Replaced `T JsonObject::get<T>(k)` with `JsonVariant JsonObject::get(k)`
* Added `JSON_STRING_SIZE()`
* ~~Replacing or removing a value now releases the memory~~
* Added `DeserializationError::code()` to be used in switch statements (issue #846)

v6.5.0-beta (2018-10-13)
-----------

* Added implicit conversion from `JsonArray` and `JsonObject` to `JsonVariant`
* Allow mixed configuration in compilation units (issue #809)
* Fixed object keys not being duplicated
* `JsonPair::key()` now returns a `JsonKey`
* Increased the default capacity of `DynamicJsonDocument`
* Fixed `JsonVariant::is<String>()` (closes #763)
* Added `JsonArrayConst`, `JsonObjectConst`, and `JsonVariantConst`
* Added copy-constructor and copy-assignment-operator for `JsonDocument` (issue #827)

v6.4.0-beta (2018-09-11)
-----------

* Copy `JsonArray` and `JsonObject`, instead of storing pointers (issue #780)
* Added `JsonVariant::to<JsonArray>()` and `JsonVariant::to<JsonObject>()`

v6.3.0-beta (2018-08-31)
-----------

* Implemented reference semantics for `JsonVariant`
* Replaced `JsonPair`'s `key` and `value` with `key()` and `value()`
* Fixed `serializeJson(obj[key], dst)` (issue #794)

> ### BREAKING CHANGES
>
> #### JsonVariant
> 
> `JsonVariant` now has a semantic similar to `JsonObject` and `JsonArray`.
> It's a reference to a value stored in the `JsonDocument`.
> As a consequence, a `JsonVariant` cannot be used as a standalone variable anymore.
>
> Old code:
>
> ```c++
> JsonVariant myValue = 42;
> ```
>
> New code:
>
> ```c++
> DynamicJsonDocument doc;
> JsonVariant myValue = doc.to<JsonVariant>();
> myValue.set(42);
> ```
>
> #### JsonPair
>
> Old code:
>
> ```c++
> for(JsonPair p : myObject) {
>   Serial.println(p.key);
>   Serial.println(p.value.as<int>());
> }
> ```
>
> New code:
>
> ```c++
> for(JsonPair p : myObject) {
>   Serial.println(p.key());
>   Serial.println(p.value().as<int>());
> }
> ```
>
> CAUTION: the key is now read only!

v6.2.3-beta (2018-07-19)
-----------

* Fixed exception when using Flash strings as object keys (issue #784)

v6.2.2-beta (2018-07-18)
-----------

* Fixed `invalid application of 'sizeof' to incomplete type '__FlashStringHelper'` (issue #783)
* Fixed `char[]` not duplicated when passed to `JsonVariant::operator[]`

v6.2.1-beta (2018-07-17)
-----------

* Fixed `JsonObject` not inserting keys of type `String` (issue #782)

v6.2.0-beta (2018-07-12)
-----------

* Disabled lazy number deserialization (issue #772)
* Fixed `JsonVariant::is<int>()` that returned true for empty strings
* Improved float serialization when `-fsingle-precision-constant` is used
* Renamed function `RawJson()` to `serialized()`
* `serializeMsgPack()` now supports values marked with `serialized()`

> ### BREAKING CHANGES
>
> #### Non quoted strings
>
> Non quoted strings are now forbidden in values, but they are still allowed in keys.
> For example, `{key:"value"}` is accepted, but `{key:value}` is not.
>
> #### Preformatted values
>
> Old code:
>
> ```c++
> object["values"] = RawJson("[1,2,3,4]");
> ```
> 
> New code:
> 
> ```c++
> object["values"] = serialized("[1,2,3,4]");
> ```

v6.1.0-beta (2018-07-02)
-----------

* Return `JsonArray` and `JsonObject` by value instead of reference (issue #309)
* Replaced `success()` with `isNull()`

> ### BREAKING CHANGES
> 
> Old code:
>
> ```c++
> JsonObject& obj = doc.to<JsonObject>();
> JsonArray& arr = obj.createNestedArray("key");
> if (!arr.success()) {
>   Serial.println("Not enough memory");
>   return;
> }
> ```
> 
> New code:
> 
> ```c++
> JsonObject obj = doc.to<JsonObject>();
> JsonArray arr = obj.createNestedArray("key");
> if (arr.isNull()) {
>   Serial.println("Not enough memory");
>   return;
> }
> ```

v6.0.1-beta (2018-06-11)
-----------

* Fixed conflicts with `isnan()` and `isinf()` macros (issue #752)

v6.0.0-beta (2018-06-07)
-----------

* Added `DynamicJsonDocument` and `StaticJsonDocument`
* Added `deserializeJson()`
* Added `serializeJson()` and `serializeJsonPretty()`
* Added `measureJson()` and `measureJsonPretty()`
* Added `serializeMsgPack()`, `deserializeMsgPack()` and `measureMsgPack()` (issue #358)
* Added example `MsgPackParser.ino` (issue #358)
* Added support for non zero-terminated strings (issue #704)
* Removed `JsonBuffer::parseArray()`, `parseObject()` and `parse()`
* Removed `JsonBuffer::createArray()` and `createObject()`
* Removed `printTo()` and `prettyPrintTo()`
* Removed `measureLength()` and `measurePrettyLength()`
* Removed all deprecated features

> ### BREAKING CHANGES
> 
> #### Deserialization
> 
> Old code:
> 
> ```c++
> DynamicJsonBuffer jb;
> JsonObject& obj = jb.parseObject(json);
> if (obj.success()) {
> 
> }
> ```
> 
> New code:
> 
> ```c++
> DynamicJsonDocument doc;
> DeserializationError error = deserializeJson(doc, json);
> if (error) {
> 
> }
> JsonObject& obj = doc.as<JsonObject>();
> ```
> 
> #### Serialization
> 
> Old code:
> 
> ```c++
> DynamicJsonBuffer jb;
> JsonObject& obj = jb.createObject();
> obj["key"] = "value";
> obj.printTo(Serial);
> ```
> 
> New code:
> 
> ```c++
> DynamicJsonDocument obj;
> JsonObject& obj = doc.to<JsonObject>();
> obj["key"] = "value";
> serializeJson(doc, Serial);
> ```

v5.13.2
-------

* Fixed `JsonBuffer::parse()` not respecting nesting limit correctly (issue #693)
* Fixed inconsistencies in nesting level counting (PR #695 from Zhenyu Wu)
* Fixed null values that could be pass to `strcmp()` (PR #745 from Mike Karlesky)
* Added macros `ARDUINOJSON_VERSION`, `ARDUINOJSON_VERSION_MAJOR`...

v5.13.1
-------

* Fixed `JsonVariant::operator|(int)` that returned the default value if the variant contained a double (issue #675)
* Allowed non-quoted key to contain underscores (issue #665)

v5.13.0
-------

* Changed the rules of string duplication (issue #658)
* `RawJson()` accepts any kind of string and obeys to the same rules for duplication
* Changed the return type of `strdup()` to `const char*` to prevent double duplication
* Marked `strdup()` as deprecated

> ### New rules for string duplication
>
> | type                       | duplication |
> |:---------------------------|:------------|
> | const char*                | no          |
> | char*                      | ~~no~~ yes  |
> | String                     | yes         |
> | std::string                | yes         |
> | const __FlashStringHelper* | yes         |
>
> These new rules make `JsonBuffer::strdup()` useless.

v5.12.0
-------

* Added `JsonVariant::operator|` to return a default value (see below)
* Added a clear error message when compiled as C instead of C++ (issue #629)
* Added detection of MPLAB XC compiler (issue #629)
* Added detection of Keil ARM Compiler (issue #629)
* Added an example that shows how to save and load a configuration file
* Reworked all other examples

> ### How to use the new feature?
>
> If you have a block like this:
>
> ```c++
> const char* ssid = root["ssid"];
> if (!ssid)
>   ssid = "default ssid";
> ```
>
> You can simplify like that:
>
> ```c++
> const char* ssid = root["ssid"] | "default ssid";
> ```

v5.11.2
-------

* Fixed `DynamicJsonBuffer::clear()` not resetting allocation size (issue #561)
* Fixed incorrect rounding for float values (issue #588)

v5.11.1
-------

* Removed dependency on `PGM_P` as Particle 0.6.2 doesn't define it (issue #546)
* Fixed warning "dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]"
* Fixed warning "floating constant exceeds range of 'float' [-Woverflow]" (issue #544)
* Fixed warning "this statement may fall through" [-Wimplicit-fallthrough=] (issue #539)
* Removed `ARDUINOJSON_DOUBLE_IS_64BITS` as it became useless.
* Fixed too many decimals places in float serialization (issue #543)

v5.11.0
-------

* Made `JsonBuffer` non-copyable (PR #524 by @luisrayas3)
* Added `StaticJsonBuffer::clear()`
* Added `DynamicJsonBuffer::clear()`

v5.10.1
-------

* Fixed IntelliSense errors in Visual Micro (issue #483)
* Fixed compilation in IAR Embedded Workbench (issue #515)
* Fixed reading "true" as a float (issue #516)
* Added `ARDUINOJSON_DOUBLE_IS_64BITS`
* Added `ARDUINOJSON_EMBEDDED_MODE`

v5.10.0
-------

* Removed configurable number of decimal places (issues #288, #427 and #506)
* Changed exponentiation thresholds to `1e7` and `1e-5` (issues #288, #427 and #506)
* `JsonVariant::is<double>()` now returns `true` for integers
* Fixed error `IsBaseOf is not a member of ArduinoJson::TypeTraits` (issue #495)
* Fixed error `forming reference to reference` (issue #495)

> ### BREAKING CHANGES :warning:
>
> | Old syntax                      | New syntax          |
> |:--------------------------------|:--------------------|
> | `double_with_n_digits(3.14, 2)` | `3.14`              |
> | `float_with_n_digits(3.14, 2)`  | `3.14f`             |
> | `obj.set("key", 3.14, 2)`       | `obj["key"] = 3.14` |
> | `arr.add(3.14, 2)`              | `arr.add(3.14)`     |
>
> | Input     | Old output | New output |
> |:----------|:-----------|:-----------|
> | `3.14159` | `3.14`     | `3.14159`  |
> | `42.0`    | `42.00`    | `42`       |
> | `0.0`     | `0.00`     | `0`        |
>
> | Expression                     | Old result | New result |
> |:-------------------------------|:-----------|:-----------|
> | `JsonVariant(42).is<int>()`    | `true`     | `true`     |
> | `JsonVariant(42).is<float>()`  | `false`    | `true`     |
> | `JsonVariant(42).is<double>()` | `false`    | `true`     |

v5.9.0
------

* Added `JsonArray::remove(iterator)` (issue #479)
* Added `JsonObject::remove(iterator)`
* Renamed `JsonArray::removeAt(size_t)` into `remove(size_t)`
* Renamed folder `include/` to `src/`
* Fixed warnings `floating constant exceeds range of float`and `floating constant truncated to zero` (issue #483)
* Removed `Print` class and converted `printTo()` to a template method (issue #276)
* Removed example `IndentedPrintExample.ino`
* Now compatible with Particle 0.6.1, thanks to Jacob Nite (issue #294 and PR #461 by @foodbag)

v5.8.4
------

* Added custom implementation of `strtod()` (issue #453)
* Added custom implementation of `strtol()` (issue #465)
* `char` is now treated as an integral type (issue #337, #370)

v5.8.3
------

* Fixed an access violation in `DynamicJsonBuffer` when memory allocation fails (issue #433)
* Added operators `==` and `!=` for two `JsonVariant`s (issue #436)
* Fixed `JsonVariant::operator[const FlashStringHelper*]` (issue #441)

v5.8.2
------

* Fixed parsing of comments (issue #421)
* Fixed ignored `Stream` timeout (issue #422)
* Made sure we don't read more that necessary (issue #422)
* Fixed error when the key of a `JsonObject` is a `char[]` (issue #423)
* Reduced code size when using `const` references
* Fixed error with string of type `unsigned char*` (issue #428)
* Added `deprecated` attribute on `asArray()`, `asObject()` and `asString()` (issue #420)

v5.8.1
------

* Fixed error when assigning a `volatile int` to a `JsonVariant` (issue #415)
* Fixed errors with Variable Length Arrays (issue #416)
* Fixed error when both `ARDUINOJSON_ENABLE_STD_STREAM` and `ARDUINOJSON_ENABLE_ARDUINO_STREAM` are set to `1`
* Fixed error "Stream does not name a type" (issue #412)

v5.8.0
------

* Added operator `==` to compare `JsonVariant` and strings (issue #402)
* Added support for `Stream` (issue #300)
* Reduced memory consumption by not duplicating spaces and comments

> ### BREAKING CHANGES :warning:
>
> `JsonBuffer::parseObject()` and  `JsonBuffer::parseArray()` have been pulled down to the derived classes `DynamicJsonBuffer` and `StaticJsonBufferBase`.
>
> This means that if you have code like:
>
> ```c++
> void myFunction(JsonBuffer& jsonBuffer);
> ```
>
> you need to replace it with one of the following:
>
> ```c++
> void myFunction(DynamicJsonBuffer& jsonBuffer);
> void myFunction(StaticJsonBufferBase& jsonBuffer);
> template<typename TJsonBuffer> void myFunction(TJsonBuffer& jsonBuffer);
> ```

v5.7.3
------

* Added an `printTo(char[N])` and `prettyPrintTo(char[N])` (issue #292)
* Added ability to set a nested value like this: `root["A"]["B"] = "C"` (issue #352)
* Renamed `*.ipp` to `*Impl.hpp` because they were ignored by Arduino IDE (issue #396)

v5.7.2
------

* Made PROGMEM available on more platforms (issue #381)
* Fixed PROGMEM causing an exception on ESP8266 (issue #383)

v5.7.1
------

* Added support for PROGMEM (issue #76)
* Fixed compilation error when index is not an `int` (issue #381)

v5.7.0
------

* Templatized all functions using `String` or `std::string`
* Removed `ArduinoJson::String`
* Removed `JsonVariant::defaultValue<T>()`
* Removed non-template `JsonObject::get()` and `JsonArray.get()`
* Fixed support for `StringSumHelper` (issue #184)
* Replaced `ARDUINOJSON_USE_ARDUINO_STRING` by `ARDUINOJSON_ENABLE_STD_STRING` and `ARDUINOJSON_ENABLE_ARDUINO_STRING` (issue #378)
* Added example `StringExample.ino` to show where `String` can be used
* Increased default nesting limit to 50 when compiled for a computer (issue #349)

> ### BREAKING CHANGES :warning:
>
> The non-template functions `JsonObject::get()` and `JsonArray.get()` have been removed. This means that you need to explicitely tell the type you expect in return.
>
> Old code:
>
> ```c++
> #define ARDUINOJSON_USE_ARDUINO_STRING 0
> JsonVariant value1 = myObject.get("myKey");
> JsonVariant value2 = myArray.get(0);
> ```
>
> New code:
>
> ```c++
> #define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
> #define ARDUINOJSON_ENABLE_STD_STRING 1
> JsonVariant value1 = myObject.get<JsonVariant>("myKey");
> JsonVariant value2 = myArray.get<JsonVariant>(0);
> ```

v5.6.7
------

* Fixed `array[idx].as<JsonVariant>()` and `object[key].as<JsonVariant>()`
* Fixed return value of `JsonObject::set()` (issue #350)
* Fixed undefined behavior in `Prettyfier` and `Print` (issue #354)
* Fixed parser that incorrectly rejected floats containing a `+` (issue #349)

v5.6.6
------

* Fixed `-Wparentheses` warning introduced in v5.6.5 (PR #335 by @nuket)
* Added `.mbedignore` for ARM mbdeb (PR #334 by @nuket)
* Fixed  `JsonVariant::success()` which didn't propagate `JsonArray::success()` nor `JsonObject::success()` (issue #342).

v5.6.5
------

* `as<char*>()` now returns `true` when input is `null` (issue #330)

v5.6.4
------

* Fixed error in float serialization (issue #324)

v5.6.3
------

* Improved speed of float serialization (about twice faster)
* Added `as<JsonArray>()` as a synonym for `as<JsonArray&>()`... (issue #291)
* Fixed `call of overloaded isinf(double&) is ambiguous` (issue #284)

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

> ### BREAKING CHANGE :warning:
>
> If you defined `ARDUINOJSON_ENABLE_STD_STREAM`, you now need to define it to `1`.

v5.0.8
------

* Made the library compatible with [PlatformIO](http://platformio.org/) (issue #181)
* Fixed `JsonVariant::is<bool>()` that was incorrectly returning false (issue #214)

v5.0.7
------

* Made library easier to use from a CMake project: simply `add_subdirectory(ArduinoJson/src)`
* Changed `String` to be a `typedef` of `std::string` (issues #142 and #161)

> ### BREAKING CHANGES :warning:
>
> - `JsonVariant(true).as<String>()` now returns `"true"` instead of `"1"`
> - `JsonVariant(false).as<String>()` now returns `"false"` instead of `"0"`

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

> ### BREAKING CHANGES :warning:
>
> - `JsonObject::add()` was renamed to `set()`
> - `JsonArray::at()` and `JsonObject::at()` were renamed to `get()`
> - Number of digits of floating point value are now set with `double_with_n_digits()`

**Personal note about the `String` class**:
Support of the `String` class has been added to the library because many people use it in their programs.
However, you should not see this as an invitation to use the `String` class.
The `String` class is **bad** because it uses dynamic memory allocation.
Compared to static allocation, it compiles to a bigger, slower program, and is less predictable.
You certainly don't want that in an embedded environment!
