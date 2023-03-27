ArduinoJson: change log
=======================

v6.21.1 (2023-03-27)
-------

* Double speed of `DynamicJsonDocument::garbageCollect()`
* Fix compatibility with GCC 5.2 (issue #1897)

v6.21.0 (2023-03-14)
-------

* Drop support for C++98/C++03. Minimum required is C++11.
* Remove `ARDUINOJSON_NAMESPACE`; use `ArduinoJson` instead.
* Make string support generic (issue #1807)

v6.20.1 (2023-02-08)
-------

* Remove explicit exclusion of `as<char*>()` and `as<char>()` (issue #1860)
  If you try to call them, you'll now get the same error message as any unsupported type.
  You could also add a custom converter for `char*` and `char`.

v6.20.0 (2022-12-26)
-------

* Add `JsonVariant::shallowCopy()` (issue #1343)
* Fix `9.22337e+18 is outside the range of representable values of type 'long'`
* Fix comparison operators for `JsonArray`, `JsonArrayConst`, `JsonObject`, and `JsonObjectConst`
* Fix lax parsing of `true`, `false`, and `null` (issue #1781)
* Remove undocumented `accept()` functions
* Rename `addElement()` to `add()`
* Remove `getElement()`, `getOrAddElement()`, `getMember()`, and `getOrAddMember()`
* Remove undocumented `JsonDocument::data()` and `JsonDocument::memoryPool()`
* Remove undocumented `JsonArrayIterator::internal()` and `JsonObjectIterator::internal()`
* Rename things in `ARDUINOJSON_NAMESPACE` to match the public names
* Add documentation to most public symbols
* Remove support for naked `char` (was deprecated since 6.18.0)

> ### BREAKING CHANGES
>
> This release hides `JsonVariant`'s functions that were only intended for internal use.
> If you were using them in your programs, you must replace with `operator[]` and `to<JsonVariant>()`, like so:
>
> ```c++
> // before
> JsonVariant a = variant.getElement(idx);
> JsonVariant b = variant.getOrAddElement(idx);
> JsonVariant c = variant.getMember(key);
> JsonVariant d = variant.getOrAddMember(key);
>
> // after
> JsonVariant a = variant[idx];
> JsonVariant b = idx < variant.size() ? variant[idx] : variant[idx].to<JsonVariant>();
> JsonVariant c = variant[key];
> JsonVariant d = variant.containsKey(key) ? variant[key] : variant[key].to<JsonVariant>();
> ```

v6.19.4 (2022-04-05)
-------

* Add `ElementProxy::memoryUsage()`
* Add `MemberProxy::memoryUsage()` (issue #1730)
* Add implicit conversion from `JsonDocument` to `JsonVariant`
* Fix comparisons operators with `const JsonDocument&`

v6.19.3 (2022-03-08)
-------

* Fix `call of overloaded 'String(const char*, int)' is ambiguous`
* Fix `JsonString` operator `==` and `!=` for non-zero-terminated string
* Fix `-Wsign-conversion` on GCC 8 (issue #1715)
* MessagePack: serialize round floats as integers (issue #1718)

v6.19.2 (2022-02-14)
-------

* Fix `cannot convert 'pgm_p' to 'const void*'` (issue #1707)

v6.19.1 (2022-01-14)
-------

* Fix crash when adding an object member in a too small `JsonDocument`
* Fix filter not working in zero-copy mode (issue #1697)

v6.19.0 (2022-01-08)
-------

* Remove `ARDUINOJSON_EMBEDDED_MODE` and assume we run on an embedded platform.  
  Dependent settings (like `ARDUINOJSON_DEFAULT_NESTING_LIMIT`) must be set individually.
* Change the default of `ARDUINOJSON_USE_DOUBLE` to `1`
* Change the default of `ARDUINOJSON_USE_LONG_LONG` to `1` on 32-bit platforms
* Add `as<JsonString>()` and `is<JsonString>()`
* Add safe bool idiom in `JsonString`
* Add support for NUL in string values (issue #1646)
* Add support for arbitrary array rank in `copyArray()`
* Add support for `char[][]` in `copyArray()`
* Remove `DeserializationError == bool` and `DeserializationError != bool`
* Renamed undocumented function `isUndefined()` to `isUnbound()`
* Fix `JsonVariant::memoryUsage()` for raw strings
* Fix `call of overloaded 'swap(BasicJsonDocument&, BasicJsonDocument&)' is ambiguous` (issue #1678)
* Fix inconsistent pool capacity between `BasicJsonDocument`'s copy and move constructors
* Fix inconsistent pool capacity between `BasicJsonDocument`'s copy and move assignments
* Fix return type of `StaticJsonDocument::operator=`
* Avoid pool reallocation in `BasicJsonDocument`'s copy assignment if capacity is the same
* Avoid including `Arduino.h` when all its features are disabled (issue #1692, PR #1693 by @paulocsanz)
* Assume `PROGMEM` is available as soon as `ARDUINO` is defined (consequence of #1693)

v6.18.5 (2021-09-28)
-------

* Set `ARDUINOJSON_EMBEDDED_MODE` to `1` on Nios II (issue #1657)

v6.18.4 (2021-09-06)
-------

* Fixed error `'dummy' may be used uninitialized` on GCC 11
* Fixed error `expected unqualified-id before 'const'` on GCC 11 (issue #1622)
* Filter: exact match takes precedence over wildcard (issue #1628)
* Fixed deserialization of `\u0000` (issue #1646)

v6.18.3 (2021-07-27)
-------

* Changed return type of `convertToJson()` and `Converter<T>::toJson()` to `void`
* Added `as<std::string_view>()` and `is<std::string_view>()`

v6.18.2 (2021-07-19)
-------

* Removed a symlink because the Arduino Library Specification forbids it

v6.18.1 (2021-07-03)
-------

* Fixed support for `volatile float` and `volatile double` (issue #1557)
* Fixed error `[Pe070]: incomplete type is not allowed` on IAR (issue #1560)
* Fixed `serializeJson(doc, String)` when allocation fails (issue #1572)
* Fixed clang-tidy warnings (issue #1574, PR #1577 by @armandas)
* Added fake class `InvalidConversion<T1,T2>` to easily identify invalid conversions (issue #1585)
* Added support for `std::string_view` (issue #1578, PR #1554 by @0xFEEDC0DE64)
* Fixed warning `definition of implicit copy constructor for 'MsgPackDeserializer' is deprecated because it has a user-declared copy assignment operator`
* Added `JsonArray::clear()` (issue #1597)
* Fixed `JsonVariant::as<unsigned>()` (issue #1601)
* Added support for ESP-IDF component build (PR #1562 by @qt1, PR #1599 by @andreaskuster)

v6.18.0 (2021-05-05)
-------

* Added support for custom converters (issue #687)
* Added support for `Printable` (issue #1444)
* Removed support for `char` values, see below (issue #1498)
* `deserializeJson()` leaves `\uXXXX` unchanged instead of returning `NotSupported`
* `deserializeMsgPack()` inserts `null` instead of returning `NotSupported`
* Removed `DeserializationError::NotSupported`
* Added `JsonVariant::is<JsonArrayConst/JsonObjectConst>()` (issue #1412)
* Added `JsonVariant::is<JsonVariant/JsonVariantConst>()` (issue #1412)
* Changed `JsonVariantConst::is<JsonArray/JsonObject>()` to return `false` (issue #1412)
* Simplified `JsonVariant::as<T>()` to always return `T` (see below)
* Updated folders list in `.mbedignore` (PR #1515 by @AGlass0fMilk)
* Fixed member-call-on-null-pointer in `getMember()` when array is empty
* `serializeMsgPack(doc, buffer, size)` doesn't add null-terminator anymore (issue #1545)
* `serializeJson(doc, buffer, size)` adds null-terminator only if there is enough room
* PlatformIO: set `build.libArchive` to `false` (PR #1550 by @askreet)

> ### BREAKING CHANGES
>
> #### Support for `char` removed
>
> We cannot cast a `JsonVariant` to a `char` anymore, so the following will break:
> ```c++
> char age = doc["age"];  //  error: no matching function for call to 'variantAs(VariantData*&)'
> ```
> Instead, you must use another integral type, such as `int8_t`:
> ```c++
> int8_t age = doc["age"];  // OK
> ```
>
> Similarly, we cannot assign from a `char` anymore, so the following will break:
> ```c++
> char age;
> doc["age"] = age;  // error: no matching function for call to 'VariantRef::set(const char&)'
> ```
> Instead, you must use another integral type, such as `int8_t`:
> ```c++
> int8_t age;
> doc["age"] = age;  // OK
> ```
> A deprecation warning with the message "Support for `char` is deprecated, use `int8_t` or `uint8_t` instead" was added to allow a smooth transition.
>
> #### `as<T>()` always returns `T`
>
> Previously, `JsonVariant::as<T>()` could return a type different from `T`.
> The most common example is `as<char*>()` that returned a `const char*`.
> While this feature simplified a few use cases, it was confusing and complicated the
> implementation of custom converters.
>
> Starting from this version, `as<T>` doesn't try to auto-correct the return type and always return `T`,
> which means that you cannot write this anymore:
>
> ```c++
> Serial.println(doc["sensor"].as<char*>());  // error: invalid conversion from 'const char*' to 'char*' [-fpermissive]
> ```
> 
> Instead, you must write:
>
> ```c++
> Serial.println(doc["sensor"].as<const char*>());  // OK
> ```
>
> A deprecation warning with the message "Replace `as<char*>()` with `as<const char*>()`" was added to allow a smooth transition.
>
> #### `DeserializationError::NotSupported` removed
>
> On a different topic, `DeserializationError::NotSupported` has been removed.
> Instead of returning this error:
>
> * `deserializeJson()` leaves `\uXXXX` unchanged (only when `ARDUINOJSON_DECODE_UNICODE` is `0`)
> * `deserializeMsgPack()` replaces unsupported values with `null`s
>
> #### Const-aware `is<T>()`
>
> Lastly, a very minor change concerns `JsonVariantConst::is<T>()`.
> It used to return `true` for `JsonArray` and `JsonOject`, but now it returns `false`.
> Instead, you must use `JsonArrayConst` and `JsonObjectConst`.

v6.17.3 (2021-02-15)
-------

* Made `JsonDocument`'s destructor protected (issue #1480)
* Added missing calls to `client.stop()` in `JsonHttpClient.ino` (issue #1485)
* Fixed error `expected ')' before 'char'` when `isdigit()` is a macro (issue #1487)
* Fixed error `definition of implicit copy constructor is deprecated` on Clang 10
* PlatformIO: set framework compatibility to `*` (PR #1490 by @maxgerhardt)

v6.17.2 (2020-11-14)
-------

* Fixed invalid conversion error in `operator|(JsonVariant, char*)` (issue #1432)
* Changed the default value of `ARDUINOJSON_ENABLE_PROGMEM` (issue #1433).
  It now checks that the `pgm_read_XXX` macros are defined before enabling `PROGMEM`.

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
