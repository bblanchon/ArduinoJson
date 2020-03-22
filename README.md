![ArduinoJson](banner.svg)

---

[![arduino-library-badge](https://www.ardu-badge.com/badge/ArduinoJson.svg?version=6.15.0)](https://www.ardu-badge.com/ArduinoJson/6.15.0)
[![Build Status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/6.x?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x)
[![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=6.x)](https://travis-ci.org/bblanchon/ArduinoJson)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/arduinojson.svg)](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson)
[![Coverage Status](https://coveralls.io/repos/github/bblanchon/ArduinoJson/badge.svg?branch=6.x)](https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x)
[![GitHub stars](https://img.shields.io/github/stars/bblanchon/ArduinoJson?style=flat)](https://github.com/bblanchon/ArduinoJson/stargazers)

ArduinoJson is a C++ JSON library for Arduino and IoT (Internet Of Things).

## Features

* [JSON deserialization](https://arduinojson.org/v6/api/json/deserializejson/)
    * [Optionally decodes UTF-16 escape sequences to UTF-8](https://arduinojson.org/v6/api/config/decode_unicode/)
    * [Optionally stores links to the input buffer (zero-copy)](https://arduinojson.org/v6/api/json/deserializejson/)
    * [Optionally supports comments in the input](https://arduinojson.org/v6/api/config/enable_comments/)
    * Optionally filters the input to keep only desired values
    * Supports single quotes as a string delimiter
    * Compatible with NDJSON and JSON Lines
* [JSON serialization](https://arduinojson.org/v6/api/json/serializejson/)
    * [Can write to a buffer or a stream](https://arduinojson.org/v6/api/json/serializejson/)
    * [Optionally indents the document (prettified JSON)](https://arduinojson.org/v6/api/json/serializejsonpretty/)
* [MessagePack serialization](https://arduinojson.org/v6/api/msgpack/serializemsgpack/)
* [MessagePack deserialization](https://arduinojson.org/v6/api/msgpack/deserializemsgpack/)
* Efficient
    * [Twice smaller than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/)
    * [Almost 10% faster than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/)
    * [Consumes roughly 10% less RAM than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/)
    * [Fixed memory allocation, no heap fragmentation](https://arduinojson.org/v6/api/jsondocument/)
    * [Optionally works without heap memory (zero malloc)](https://arduinojson.org/v6/api/staticjsondocument/)
* Versatile
    * [Supports custom allocators (to use external RAM chip, for example)](https://arduinojson.org/v6/how-to/use-external-ram-on-esp32/)
    * Supports [Arduino's `String`](https://arduinojson.org/v6/api/config/enable_arduino_string/) and [STL's `std::string`](https://arduinojson.org/v6/api/config/enable_std_string/)
    * Supports Arduino's `Stream` and [STL's `std::istream`/`std::ostream`](https://arduinojson.org/v6/api/config/enable_std_stream/)
    * [Supports Flash strings](https://arduinojson.org/v6/api/config/enable_progmem/)
* Portable
    * Usable on any C++ project (not limited to Arduino)
    * Compatible with C++98
    * Zero warnings with `-Wall -Wextra -pedantic` and `/W4`
    * [Header-only library](https://en.wikipedia.org/wiki/Header-only)
    * Works with virtually any board
        * Arduino boards: [Uno](https://amzn.to/38aL2ik), [Due](https://amzn.to/36YkWi2), [Micro](https://amzn.to/35WkdwG), [Nano](https://amzn.to/2QTvwRX), [Mega](https://amzn.to/36XWhuf), [Yun](https://amzn.to/30odURc), [Leonardo](https://amzn.to/36XWjlR)...
        * Espressif chips: [ESP8266](https://amzn.to/36YluV8), [ESP32](https://amzn.to/2G4pRCB)
        * Lolin (WeMos) boards: [D1 mini](https://amzn.to/2QUpz7q), [D1 Mini Pro](https://amzn.to/36UsGSs)...
        * Teensy boards: [4.0](https://amzn.to/30ljXGq), [3.2](https://amzn.to/2FT0EuC), [2.0](https://amzn.to/2QXUMXj) 
        * Particle boards: [Argon](https://amzn.to/2FQHa9X), [Boron](https://amzn.to/36WgLUd), [Electron](https://amzn.to/30vEc4k), [Photon](https://amzn.to/387F9Cd)...
        * Texas Instruments boards: [MSP430](https://amzn.to/30nJWgg)...
    * Tested on all major development environments
        * [Arduino IDE](https://www.arduino.cc/en/Main/Software)
        * [Atmel Studio](http://www.atmel.com/microsite/atmel-studio/)
        * [Atollic TrueSTUDIO](https://atollic.com/truestudio/)
        * [Energia](http://energia.nu/)
        * [IAR Embedded Workbench](https://www.iar.com/iar-embedded-workbench/)
        * [Keil uVision](http://www.keil.com/)
        * [MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide)
        * [PlatformIO](http://platformio.org/)
        * [Sloeber plugin for Eclipse](https://eclipse.baeyens.it/)
        * [Visual Micro](http://www.visualmicro.com/)
        * [Visual Studio](https://www.visualstudio.com/)
    * [Even works with online compilers like wandbox.org](https://wandbox.org/permlink/t7KP7I6dVuLhqzDl)
* Well designed
    * [Elegant API](http://127.0.0.1:4000/v6/example/)
    * [Thread-safe](https://en.wikipedia.org/wiki/Thread_safety)
    * Self-contained (no external dependency)
    * `const` friendly
    * `for` friendly
    * [TMP friendly](https://en.wikipedia.org/wiki/Template_metaprogramming)
* Well tested
    * [Unit test coverage close to 100%](https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x)
    * Continuously tested on
        * [Visual Studio 2010, 2012, 2013, 2015, 2017, 2019](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x)
        * [GCC 4.4, 4.6, 4.7, 4.8, 4.9, 5, 6, 7, 8](https://travis-ci.org/bblanchon/ArduinoJson)
        * [Clang 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 5.0, 6.0, 7, 8](https://travis-ci.org/bblanchon/ArduinoJson)
    * [Continuously fuzzed with Google OSS Fuzz](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson)
* Well documented
    * [Tutorials](https://arduinojson.org/v6/doc/deserialization/)
    * [Examples](https://arduinojson.org/v6/example/)
    * [How-tos](https://arduinojson.org/v6/example/)
    * [FAQ](https://arduinojson.org/v6/faq/)
    * [Book](https://arduinojson.org/book/)
* Vibrant user community
    * Most popular of all Arduino libraries on [GitHub](https://github.com/search?o=desc&q=arduino+library&s=stars&type=Repositories) and [PlatformIO](https://platformio.org/lib/search)
    * [Used in hundreds of projects](https://www.hackster.io/search?i=projects&q=arduinojson)
    * [Responsive support](https://github.com/bblanchon/ArduinoJson/issues?q=is%3Aissue+is%3Aclosed)

## Quickstart

### Deserialization

Here is a program that parses a JSON document with ArduinoJson.

```c++
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

DynamicJsonDocument doc(1024);
deserializeJson(doc, json);

const char* sensor = doc["sensor"];
long time          = doc["time"];
double latitude    = doc["data"][0];
double longitude   = doc["data"][1];
```

See the [tutorial on arduinojson.org](https://arduinojson.org/doc/decoding/?utm_source=github&utm_medium=readme)

### Serialization

Here is a program that generates a JSON document with ArduinoJson:

```c++
DynamicJsonDocument doc(1024);

doc["sensor"] = "gps";
doc["time"]   = 1351824120;

JsonArray data = doc.createNestedArray("data");
data.add(48.756080);
data.add(2.302038);

serializeJson(doc, Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

See the [tutorial on arduinojson.org](https://arduinojson.org/doc/encoding/?utm_source=github&utm_medium=readme)

## Support the project

Do you like this library? Please [star this project on GitHub](https://github.com/bblanchon/ArduinoJson/stargazers)!

What? You don't like it but you *love* it?
We don't take donations anymore, but [we sell a book](https://arduinojson.org/book/?utm_source=github&utm_medium=readme), so you can help and learn at the same time!
