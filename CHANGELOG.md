Arduino JSON library: change log
================================

v2.0
----

* Added JSON encoding.

**Breaking change**: you need to add the following line at the top of your program.

	using namespace ArduinoJson::Parser;

v1.2
----

* Example: changed `char[] json` into `char json[]`. Damn it C# !

v1.1
----

* Example: changed `char* json` into `char[] json` so that the bytes are not write protected
* Fixed parsing bug when the JSON contains multi-dimensional arrays

v1.0 
----

Initial release