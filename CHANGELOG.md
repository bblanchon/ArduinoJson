ArduinoJson: change log
=======================

HEAD
----

* Remove `BasicJsonDocument`
* Remove `StaticJsonDocument`
* Add abstract `Allocator` class
* Merge `DynamicJsonDocument` with `JsonDocument`
* Remove `JSON_ARRAY_SIZE()`, `JSON_OBJECT_SIZE()`, and `JSON_STRING_SIZE()`
* Remove `ARDUINOJSON_ENABLE_STRING_DEDUPLICATION` (string deduplication cannot be enabled anymore)
* Remove `JsonDocument::capacity()`
* Store the strings in the heap
* Reference-count shared strings
* Always store `serialized("string")` by copy (#1915)
