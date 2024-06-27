ArduinoJson: change log
=======================

v7.1.0 (2024-06-27)
------

* Add `ARDUINOJSON_STRING_LENGTH_SIZE` to the namespace name
* Add support for MsgPack binary (PR #2078 by @Sanae6)
* Add support for MsgPack extension
* Make string support even more generic (PR #2084 by @d-a-v)
* Optimize `deserializeMsgPack()`
* Allow using a `JsonVariant` as a key or index (issue #2080)
  Note: works only for reading, not for writing
* Support `ElementProxy` and `MemberProxy` in `JsonDocument`'s constructor
* Don't add partial objects when allocation fails (issue #2081)
* Read MsgPack's 64-bit integers even if `ARDUINOJSON_USE_LONG_LONG` is `0`
  (they are set to `null` if they don't fit in a `long`)

v7.0.4 (2024-03-12)
------

* Make `JSON_STRING_SIZE(N)` return `N+1` to fix third-party code (issue #2054)

v7.0.3 (2024-02-05)
------

* Improve error messages when using `char` or `char*` (issue #2043)
* Reduce stack consumption (issue #2046)
* Fix compatibility with GCC 4.8 (issue #2045)

v7.0.2 (2024-01-19)
------

* Fix assertion `poolIndex < count_` after `JsonDocument::clear()` (issue #2034)

v7.0.1 (2024-01-10)
------

* Fix "no matching function" with `JsonObjectConst::operator[]` (issue #2019)
* Remove unused files in the PlatformIO package
* Fix `volatile bool` serialized as `1` or `0` instead of `true` or `false` (issue #2029)

v7.0.0 (2024-01-03)
------

* Remove `BasicJsonDocument`
* Remove `StaticJsonDocument`
* Add abstract `Allocator` class
* Merge `DynamicJsonDocument` with `JsonDocument`
* Remove `JSON_ARRAY_SIZE()`, `JSON_OBJECT_SIZE()`, and `JSON_STRING_SIZE()`
* Remove `ARDUINOJSON_ENABLE_STRING_DEDUPLICATION` (string deduplication cannot be disabled anymore)
* Remove `JsonDocument::capacity()`
* Store the strings in the heap
* Reference-count shared strings
* Always store `serialized("string")` by copy (#1915)
* Remove the zero-copy mode of `deserializeJson()` and `deserializeMsgPack()`
* Fix double lookup in `to<JsonVariant>()`
* Fix double call to `size()` in `serializeMsgPack()`
* Include `ARDUINOJSON_SLOT_OFFSET_SIZE` in the namespace name
* Remove `JsonVariant::shallowCopy()`
* `JsonDocument`'s capacity grows as needed, no need to pass it to the constructor anymore
* `JsonDocument`'s allocator is not monotonic anymore, removed values get recycled
* Show a link to the documentation when user passes an unsupported input type
* Remove `JsonDocument::memoryUsage()`
* Remove `JsonDocument::garbageCollect()`
* Add `deserializeJson(JsonVariant, ...)` and `deserializeMsgPack(JsonVariant, ...)` (#1226)
* Call `shrinkToFit()` in `deserializeJson()` and `deserializeMsgPack()`
* `serializeJson()` and `serializeMsgPack()` replace the content of `std::string` and `String` instead of appending to it
* Replace `add()` with `add<T>()` (`add(T)` is still supported)
* Remove `createNestedArray()` and `createNestedObject()` (use `to<JsonArray>()` and `to<JsonObject>()` instead)

> ### BREAKING CHANGES
>
> As every major release, ArduinoJson 7 introduces several breaking changes.
> I added some stubs so that most existing programs should compile, but I highty recommend you upgrade your code.
>
> #### `JsonDocument`
> 
> In ArduinoJson 6, you could allocate the memory pool on the stack (with `StaticJsonDocument`) or in the heap (with `DynamicJsonDocument`).  
> In ArduinoJson 7, the memory pool is always allocated in the heap, so `StaticJsonDocument` and `DynamicJsonDocument` have been merged into `JsonDocument`.
>
> In ArduinoJson 6, `JsonDocument` had a fixed capacity; in ArduinoJson 7, it has an elastic capacity that grows as needed.
> Therefore, you don't need to specify the capacity anymore, so the macros `JSON_ARRAY_SIZE()`, `JSON_OBJECT_SIZE()`, and `JSON_STRING_SIZE()` have been removed.
>
> ```c++
> // ArduinoJson 6
> StaticJsonDocument<256> doc;
> // or
> DynamicJsonDocument doc(256);
> 
> // ArduinoJson 7
> JsonDocument doc;
> ```
>
> In ArduinoJson 7, `JsonDocument` reuses released memory, so `garbageCollect()` has been removed.  
> `shrinkToFit()` is still available and releases the over-allocated memory.
>
> Due to a change in the implementation, it's not possible to store a pointer to a variant from another `JsonDocument`, so `shallowCopy()` has been removed.
> 
> In ArduinoJson 6, the meaning of `memoryUsage()` was clear: it returned the number of bytes used in the memory pool.  
> In ArduinoJson 7, the meaning of `memoryUsage()` would be ambiguous, so it has been removed.
>
> #### Custom allocators
>
> In ArduinoJson 6, you could specify a custom allocator class as a template parameter of `BasicJsonDocument`.  
> In ArduinoJson 7, you must inherit from `ArduinoJson::Allocator` and pass a pointer to an instance of your class to the constructor of `JsonDocument`.
>
> ```c++
> // ArduinoJson 6
> class MyAllocator {
>   // ...
> };
> BasicJsonDocument<MyAllocator> doc(256);
>
> // ArduinoJson 7
> class MyAllocator : public ArduinoJson::Allocator {
>   // ...
> };
> MyAllocator myAllocator;
> JsonDocument doc(&myAllocator);
> ```
>
> #### `createNestedArray()` and `createNestedObject()`
>
> In ArduinoJson 6, you could create a nested array or object with `createNestedArray()` and `createNestedObject()`.  
> In ArduinoJson 7, you must use `add<T>()` or `to<T>()` instead.
>
> For example, to create `[[],{}]`, you would write:
>
> ```c++
> // ArduinoJson 6
> arr.createNestedArray();
> arr.createNestedObject();
>
> // ArduinoJson 7
> arr.add<JsonArray>();
> arr.add<JsonObject>();
> ```
>
> And to create `{"array":[],"object":{}}`, you would write:
>
> ```c++
> // ArduinoJson 6
> obj.createNestedArray("array");
> obj.createNestedObject("object");
>
> // ArduinoJson 7
> obj["array"].to<JsonArray>();
> obj["object"].to<JsonObject>();
> ```
