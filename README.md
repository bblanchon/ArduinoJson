![ArduinoJson](banner.svg)

---

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/bblanchon/ArduinoJson/Continuous%20Integration?logo=github)](https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22+branch%3A6.x)
[![Continuous Integration](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/6.x?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x)
[![Fuzzing Status](https://oss-fuzz-build-logs.storage.googleapis.com/badges/arduinojson.svg)](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson)
[![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/bblanchon/ArduinoJson?label=quality&logo=lgtm)](https://lgtm.com/projects/g/bblanchon/ArduinoJson/)
[![Coveralls branch](https://img.shields.io/coveralls/github/bblanchon/ArduinoJson/6.x?logo=coveralls)](https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x)  
[![Arduino Library Manager](https://img.shields.io/static/v1?label=Arduino&message=v6.19.3&logo=arduino&logoColor=white&color=blue)](https://www.ardu-badge.com/ArduinoJson/6.19.3)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/bblanchon/library/ArduinoJson.svg?version=6.19.3)](https://registry.platformio.org/packages/libraries/bblanchon/ArduinoJson?version=6.19.3) 
[![GitHub stars](https://img.shields.io/github/stars/bblanchon/ArduinoJson?style=flat&logo=github)](https://github.com/bblanchon/ArduinoJson/stargazers)
[![GitHub Sponsors](https://img.shields.io/github/sponsors/bblanchon?logo=github)](https://github.com/sponsors/bblanchon)

ArduinoJson is a C++ JSON library for Arduino and IoT (Internet Of Things).

## Features

* [JSON deserialization](https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme)
    * [Optionally decodes UTF-16 escape sequences to UTF-8](https://arduinojson.org/v6/api/config/decode_unicode/?utm_source=github&utm_medium=readme)
    * [Optionally stores links to the input buffer (zero-copy)](https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme)
    * [Optionally supports comments in the input](https://arduinojson.org/v6/api/config/enable_comments/?utm_source=github&utm_medium=readme)
    * [Optionally filters the input to keep only desired values](https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme#filtering)
    * Supports single quotes as a string delimiter
    * Compatible with [NDJSON](http://ndjson.org/) and [JSON Lines](https://jsonlines.org/)
* [JSON serialization](https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme)
    * [Can write to a buffer or a stream](https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme)
    * [Optionally indents the document (prettified JSON)](https://arduinojson.org/v6/api/json/serializejsonpretty/?utm_source=github&utm_medium=readme)
* [MessagePack serialization](https://arduinojson.org/v6/api/msgpack/serializemsgpack/?utm_source=github&utm_medium=readme)
* [MessagePack deserialization](https://arduinojson.org/v6/api/msgpack/deserializemsgpack/?utm_source=github&utm_medium=readme)
* Efficient
    * [Twice smaller than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme)
    * [Almost 10% faster than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme)
    * [Consumes roughly 10% less RAM than the "official" Arduino_JSON library](https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme)
    * [Fixed memory allocation, no heap fragmentation](https://arduinojson.org/v6/api/jsondocument/?utm_source=github&utm_medium=readme)
    * [Optionally works without heap memory (zero malloc)](https://arduinojson.org/v6/api/staticjsondocument/?utm_source=github&utm_medium=readme)
    * [Deduplicates strings](https://arduinojson.org/news/2020/08/01/version-6-16-0/?utm_source=github&utm_medium=readme)
* Versatile
    * Supports [custom allocators (to use external RAM chip, for example)](https://arduinojson.org/v6/how-to/use-external-ram-on-esp32/?utm_source=github&utm_medium=readme)
    * Supports [`String`](https://arduinojson.org/v6/api/config/enable_arduino_string/?utm_source=github&utm_medium=readme), [`std::string`](https://arduinojson.org/v6/api/config/enable_std_string/?utm_source=github&utm_medium=readme), and [`std::string_view`](https://arduinojson.org/v6/api/config/enable_string_view/?utm_source=github&utm_medium=readme)
    * Supports [`Stream`](https://arduinojson.org/v6/api/config/enable_arduino_stream/?utm_source=github&utm_medium=readme) and [`std::istream`/`std::ostream`](https://arduinojson.org/v6/api/config/enable_std_stream/?utm_source=github&utm_medium=readme)
    * Supports [Flash strings](https://arduinojson.org/v6/api/config/enable_progmem/?utm_source=github&utm_medium=readme)
    * Supports [custom readers](https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme#custom-reader) and [custom writers](https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme#custom-writer)
    * Supports [custom converters](https://arduinojson.org/news/2021/05/04/version-6-18-0/?utm_source=github&utm_medium=readme)
* Portable
    * Usable on any C++ project (not limited to Arduino)
    * Compatible with C++98, C++11, C++14 and C++17
    * Zero warnings with `-Wall -Wextra -pedantic` and `/W4`
    * [Header-only library](https://en.wikipedia.org/wiki/Header-only)
    * Works with virtually any board
        * Arduino boards: [Uno](https://amzn.to/38aL2ik), [Due](https://amzn.to/36YkWi2), [Micro](https://amzn.to/35WkdwG), [Nano](https://amzn.to/2QTvwRX), [Mega](https://amzn.to/36XWhuf), [Yun](https://amzn.to/30odURc), [Leonardo](https://amzn.to/36XWjlR)...
        * Espressif chips: [ESP8266](https://amzn.to/36YluV8), [ESP32](https://amzn.to/2G4pRCB)
        * Lolin (WeMos) boards: [D1 mini](https://amzn.to/2QUpz7q), [D1 Mini Pro](https://amzn.to/36UsGSs)...
        * Teensy boards: [4.0](https://amzn.to/30ljXGq), [3.2](https://amzn.to/2FT0EuC), [2.0](https://amzn.to/2QXUMXj)
        * Particle boards: [Argon](https://amzn.to/2FQHa9X), [Boron](https://amzn.to/36WgLUd), [Electron](https://amzn.to/30vEc4k), [Photon](https://amzn.to/387F9Cd)...
        * Texas Instruments boards: [MSP430](https://amzn.to/30nJWgg)...
        * Soft cores: [Nios II](https://en.wikipedia.org/wiki/Nios_II)...
    * Tested on all major development environments
        * [Arduino IDE](https://www.arduino.cc/en/Main/Software)
        * [Atmel Studio](http://www.atmel.com/microsite/atmel-studio/)
        * [Atollic TrueSTUDIO](https://atollic.com/truestudio/)
        * [Energia](http://energia.nu/)
        * [IAR Embedded Workbench](https://www.iar.com/iar-embedded-workbench/)
        * [Keil uVision](http://www.keil.com/)
        * [MPLAB X IDE](http://www.microchip.com/mplab/mplab-x-ide)
        * [Particle](https://www.particle.io/)
        * [PlatformIO](http://platformio.org/)
        * [Sloeber plugin for Eclipse](https://eclipse.baeyens.it/)
        * [Visual Micro](http://www.visualmicro.com/)
        * [Visual Studio](https://www.visualstudio.com/)
    * [Even works with online compilers like wandbox.org](https://wandbox.org/permlink/RlZSKy17DjJ6HcdN)
    * [CMake friendly](https://arduinojson.org/v6/how-to/use-arduinojson-with-cmake/?utm_source=github&utm_medium=readme)
* Well designed
    * [Elegant API](http://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme)
    * [Thread-safe](https://en.wikipedia.org/wiki/Thread_safety)
    * Self-contained (no external dependency)
    * `const` friendly
    * [`for` friendly](https://arduinojson.org/v6/api/jsonobject/begin_end/?utm_source=github&utm_medium=readme)
    * [TMP friendly](https://en.wikipedia.org/wiki/Template_metaprogramming)
    * Handles [integer overflows](https://arduinojson.org/v6/api/jsonvariant/as/?utm_source=github&utm_medium=readme#integer-overflows)
* Well tested
    * [Unit test coverage close to 100%](https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x)
    * Continuously tested on
        * [Visual Studio 2010, 2012, 2013, 2015, 2017, 2019, 2022](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x)
        * [GCC 4.4, 4.6, 4.7, 4.8, 4.9, 5, 6, 7, 8, 9, 10, 11](https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22)
        * [Clang 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 5.0, 6.0, 7, 8, 9, 10](https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22)
    * [Continuously fuzzed with Google OSS Fuzz](https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson)
    * Passes all default checks of [clang-tidy](https://releases.llvm.org/10.0.0/tools/clang/tools/extra/docs/clang-tidy/)
* Well documented
    * [Tutorials](https://arduinojson.org/v6/doc/deserialization/?utm_source=github&utm_medium=readme)
    * [Examples](https://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme)
    * [How-tos](https://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme)
    * [FAQ](https://arduinojson.org/v6/faq/?utm_source=github&utm_medium=readme)
    * [Troubleshooter](https://arduinojson.org/v6/troubleshooter/?utm_source=github&utm_medium=readme)
    * [Book](https://arduinojson.org/book/?utm_source=github&utm_medium=readme)
    * [Changelog](CHANGELOG.md)
* Vibrant user community
    * Most popular of all Arduino libraries on [GitHub](https://github.com/search?o=desc&q=arduino+library&s=stars&type=Repositories)
    * [Used in hundreds of projects](https://www.hackster.io/search?i=projects&q=arduinojson)
    * [Responsive support](https://github.com/bblanchon/ArduinoJson/issues?q=is%3Aissue+is%3Aclosed)
    * [Discord server](https://discord.gg/DzN6hHHD4h)

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
doc["data"][0] = 48.756080;
doc["data"][1] = 2.302038;

serializeJson(doc, Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

See the [tutorial on arduinojson.org](https://arduinojson.org/doc/encoding/?utm_source=github&utm_medium=readme)

## Sponsors

ArduinoJson is thankful to its sponsors. Please give them a visit; they deserve it!

<p>
  <a href="https://techexplorations.com/" rel="sponsored">
    <img alt="Tech Explorations" src="https://arduinojson.org/images/2021/10/techexplorations.png" width="200">
  </a>
  <a href="https://www.programmingelectronics.com/" rel="sponsored">
    <img src="https://arduinojson.org/images/2021/10/programmingeleactronicsacademy.png" alt="Programming Electronics Academy" width="200">
  </a>
</p>
<p>
  <a href="https://github.com/1technophile" rel="sponsored">
    <img alt="1technophile" src="https://avatars.githubusercontent.com/u/12672732?s=40&v=4">
  </a>
</p>

If you run a commercial project that embeds ArduinoJson, think about [sponsoring the library's development](https://github.com/sponsors/bblanchon): it ensures the code that your products rely on stays actively maintained. It can also give your project some exposure to the makers' community.

If you are an individual user and want to support the development (or give a sign of appreciation), consider purchasing the book [Mastering ArduinoJson](https://arduinojson.org/book/)&nbsp;❤, or simply [cast a star](https://github.com/bblanchon/ArduinoJson/stargazers)&nbsp;⭐.
