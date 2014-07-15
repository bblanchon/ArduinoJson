Arduino JSON: change log
========================

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