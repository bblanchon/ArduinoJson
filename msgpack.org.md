MessagePack for Arduino
=======================

[![arduino-library-badge](https://www.ardu-badge.com/badge/ArduinoJson.svg?version=6.12.0)](https://www.ardu-badge.com/ArduinoJson/6.12.0)
[![Build Status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/6.x?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x)
[![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=6.x)](https://travis-ci.org/bblanchon/ArduinoJson)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/arduinojson.svg)](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson)
[![Coverage Status](https://coveralls.io/repos/github/bblanchon/ArduinoJson/badge.svg?branch=6.x)](https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x)
[![GitHub stars](https://img.shields.io/github/stars/bblanchon/ArduinoJson?style=flat)](https://github.com/bblanchon/ArduinoJson/stargazers)

[ArduinoJson](https://arduinojson.org) supports [MessagePack](http://msgpack.org/) serialization and deserialization.

It supports all [MessagePack](http://msgpack.org/) features except [bin format](https://github.com/msgpack/msgpack/blob/master/spec.md#bin-format-family) and [timestamp](https://github.com/msgpack/msgpack/blob/master/spec.md#timestamp-extension-type).

How to install?
---------------

To install [ArduinoJson](https://arduinojson.org), you can either:

* install the library from the [Arduino Library Manager](https://www.ardu-badge.com/ArduinoJson/6.12.0),
* install the library from the [Particle Web IDE](https://build.particle.io/libs/ArduinoJson/),
* download the single header from [the release page](https://github.com/bblanchon/ArduinoJson/releases/latest/).

See also: [How to install ArduinoJson](https://arduinojson.org/v6/doc/installation/) on [arduinojson.org](https://arduinojson.org).

How to serialize to MsgPack?
----------------------------

```c++
StaticJsonDocument<200> doc;
doc["hello"] = "world";
serializeMsgPack(doc, Serial);
```

See also: [serializeMsgPack()](https://arduinojson.org/v6/api/msgpack/serializemsgpack/) on [arduinojson.org](https://arduinojson.org).

How to deserialize from MsgPack?
--------------------------------

```c++
StaticJsonDocument<200> doc;
deserializeMsgPack(doc, input);
const char* hello = doc["hello"];
```

See also: [deserializeMsgPack()](https://arduinojson.org/v6/api/msgpack/deserializemsgpack/) and [MsgPackParser.ino](https://arduinojson.org/v6/example/msgpack-parser/) on [arduinojson.org](https://arduinojson.org).
