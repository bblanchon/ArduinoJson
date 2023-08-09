ArduinoJson: change log
=======================

HEAD
----

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
