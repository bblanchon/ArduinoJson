![Banner]

---

[![Badge GitHub Workflow]][Github Workflow]
[![Badge Continuous Integration]][Continuous Integration]
[![Badge Fuzzing Status]][Fuzzing Status]
[![Badge LGTM Grade]][LGTM Grade]
[![Badge Coveralls Branch]][Coveralls Branch] <br>
[![Badge Library Manager]][Library Manager]
[![Badge PlatformIO Registry]][PlatformIO Registry]
[![Badge Stars]][Stars]
[![Badge Sponsors]][Sponsors]

ArduinoJson is a C++ JSON library for Arduino and IoT (Internet Of Things).

## Features

* [JSON Deserialization]
    * [Optionally decodes UTF-16 escape sequences to UTF-8][UTF-16 To UTF-8]
    * [Optionally stores links to the input buffer (zero-copy)][Links In Buffer]
    * [Optionally supports comments in the input][Comment In Input]
    * [Optionally filters the input to keep only desired values][Filter Input]
    * Supports single quotes as a string delimiter
    * Compatible with [NDJSON] and [JSON Lines]
* [JSON Serialization]
    * [Can write to a buffer or a stream][Write To Buffer / Stream]
    * [Optionally indents the document (prettified JSON)][Prettify JSON]
* [MessagePack Serialization]
* [MessagePack Deserialization]
* Efficient
    * [Twice smaller than the "official" Arduino_JSON library][Half The Size]
    * [Almost 10% faster than the "official" Arduino_JSON library][10% Faster]
    * [Consumes roughly 10% less RAM than the "official" Arduino_JSON library][10% Less RAM]
    * [Fixed memory allocation, no heap fragmentation][Fixed Memory]
    * [Optionally works without heap memory (zero malloc)][Optional Heap]
    * [Deduplicates Strings]
* Versatile
    * Supports [custom allocators (to use external RAM chip, for example)][Custom Allocators]
    * Supports [`String`], [`std::string`], and [`std::string_view`]
    * Supports [`Stream`] and [`std::istream`/`std::ostream`]
    * Supports [Flash Strings]
    * Supports [Custom Readers] and [Custom Writers]
    * Supports [Custom Converters]
* Portable
    * Usable on any C++ project (not limited to Arduino)
    * Compatible with C++98, C++11, C++14 and C++17
    * Zero warnings with `-Wall -Wextra -pedantic` and `/W4`
    * [Header-only Library]
    * Works with virtually any board
        * Arduino boards: [Uno], [Due], [Micro], [Nano], [Mega], [Yun], [Leonardo]...
        * Espressif chips: [ESP8266], [ESP32]
        * Lolin (WeMos) boards: [D1 mini], [D1 Mini Pro]...
        * [Teensy 4.0], [Teensy 3.2], [Teensy 2.0]
        * Particle boards: [Argon], [Boron], [Electron], [Photon]...
        * Texas Instruments boards: [MSP430]...
        * Soft cores: [Nios II]...
    * Tested on all major development environments
        * [Arduino IDE]
        * [Atmel Studio]
        * [Atollic TrueSTUDIO]
        * [Energia]
        * [IAR Embedded Workbench][IAR]
        * [Keil uVision]
        * [MPLAB X IDE]
        * [Particle]
        * [PlatformIO]
        * [Sloeber plugin for Eclipse][Sloeber Plugin]
        * [Visual Micro]
        * [Visual Studio]
    * [Even works with online compilers like wandbox.org][Wandbox]
    * [CMake friendly]
* Well designed
    * [Elegant API]
    * [Thread-safe]
    * Self-contained (no external dependency)
    * `const` friendly
    * [`for` friendly]
    * [TMP friendly]
    * Handles [integer overflows]
* Well tested
    * [Unit test coverage close to 100%][100% Coverage]
    * Continuously tested on
        * [Visual Studio 2010, 2012, 2013, 2015, 2017, 2019, 2022][VS Tests]
        * [GCC 4.4, 4.6, 4.7, 4.8, 4.9, 5, 6, 7, 8, 9, 10, 11][GCC Tests]
        * [Clang 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 5.0, 6.0, 7, 8, 9, 10][Clang Tests]
    * [Continuously fuzzed with Google OSS Fuzz][Continuous Fuzzing]
    * Passes all default checks of [clang-tidy]
* Well documented
    * [Tutorials]
    * [Examples]
    * [How-tos]
    * [FAQ]
    * [Troubleshooter]
    * [Book]
    * [Changelog]
* Vibrant user community
    * Most popular of all Arduino libraries on [GitHub]
    * [Used in hundreds of projects][Widely Used]
    * [Responsive support]
    * [Discord server][Discord]

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

See the [tutorial on arduinojson.org][Tutorial Decoding]

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

See the [tutorial on arduinojson.org][Tutorial Encoding]

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

If you run a commercial project that embeds ArduinoJson, think about [sponsoring the library's development][Become Sponsor]: it ensures the code that your products rely on stays actively maintained. It can also give your project some exposure to the makers' community.

If you are an individual user and want to support the development (or give a sign of appreciation), consider purchasing the book [Mastering ArduinoJson]&nbsp;❤, or simply [cast a star]&nbsp;⭐.


<!----------------------------------------------------------------------------->

[Banner]: Resources/Banner.svg


[Badge GitHub Workflow]: https://img.shields.io/github/workflow/status/bblanchon/ArduinoJson/Continuous%20Integration?logo=github
[Badge Continuous Integration]: https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/6.x?svg=true
[Badge Fuzzing Status]: https://oss-fuzz-build-logs.storage.googleapis.com/badges/arduinojson.svg
[Badge LGTM Grade]: https://img.shields.io/lgtm/grade/cpp/github/bblanchon/ArduinoJson?label=quality&logo=lgtm
[Badge Coveralls Branch]: https://img.shields.io/coveralls/github/bblanchon/ArduinoJson/6.x?logo=coveralls
[Badge Library Manager]: https://img.shields.io/static/v1?label=Arduino&message=v6.19.4&logo=arduino&logoColor=white&color=blue
[Badge PlatformIO Registry]: https://badges.registry.platformio.org/packages/bblanchon/library/ArduinoJson.svg?version=6.19.4
[Badge Stars]: https://img.shields.io/github/stars/bblanchon/ArduinoJson?style=flat&logo=github
[Badge Sponsors]: https://img.shields.io/github/sponsors/bblanchon?logo=github


[GitHub Workflow]: https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22+branch%3A6.x
[Continuous Integration]: https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x
[Fuzzing Status]: https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson
[LGTM Grade]: https://lgtm.com/projects/g/bblanchon/ArduinoJson/
[Coveralls Branch]: https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x
[Library Manager]: https://www.ardu-badge.com/ArduinoJson/6.19.4
[PlatformIO Registry]: https://registry.platformio.org/packages/libraries/bblanchon/ArduinoJson?version=6.19.4
[Stars]: https://github.com/bblanchon/ArduinoJson/stargazers
[Sponsors]: https://github.com/sponsors/bblanchon


[JSON Deserialization]: https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme
[UTF-16 To UTF-8]: https://arduinojson.org/v6/api/config/decode_unicode/?utm_source=github&utm_medium=readme
[Links In Buffer]: https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme
[Comment In Input]: https://arduinojson.org/v6/api/config/enable_comments/?utm_source=github&utm_medium=readme
[Filter Input]: https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme#filtering
[NDJSON]: http://ndjson.org/
[JSON Lines]: https://jsonlines.org/
[JSON Serialization]: https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme
[Write To Buffer / Stream]: https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme
[Prettify JSON]: https://arduinojson.org/v6/api/json/serializejsonpretty/?utm_source=github&utm_medium=readme
[MessagePack Serialization]: https://arduinojson.org/v6/api/msgpack/serializemsgpack/?utm_source=github&utm_medium=readme
[MessagePack Deserialization]: https://arduinojson.org/v6/api/msgpack/deserializemsgpack/?utm_source=github&utm_medium=readme
[Half The Size]: https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme
[10% Faster]: https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme
[10% Less RAM]: https://arduinojson.org/2019/11/19/arduinojson-vs-arduino_json/?utm_source=github&utm_medium=readme
[Fixed Memory]: https://arduinojson.org/v6/api/jsondocument/?utm_source=github&utm_medium=readme
[Optional Heap]: https://arduinojson.org/v6/api/staticjsondocument/?utm_source=github&utm_medium=readme
[Deduplicates Strings]: https://arduinojson.org/news/2020/08/01/version-6-16-0/?utm_source=github&utm_medium=readme
[Custom Allocators]: https://arduinojson.org/v6/how-to/use-external-ram-on-esp32/?utm_source=github&utm_medium=readme
[`String`]: https://arduinojson.org/v6/api/config/enable_arduino_string/?utm_source=github&utm_medium=readme
[`std::string`]: https://arduinojson.org/v6/api/config/enable_std_string/?utm_source=github&utm_medium=readme
[`std::string_view`]: https://arduinojson.org/v6/api/config/enable_string_view/?utm_source=github&utm_medium=readme
[`Stream`]: https://arduinojson.org/v6/api/config/enable_arduino_stream/?utm_source=github&utm_medium=readme
[`std::istream`/`std::ostream`]: https://arduinojson.org/v6/api/config/enable_std_stream/?utm_source=github&utm_medium=readme
[Flash strings]: https://arduinojson.org/v6/api/config/enable_progmem/?utm_source=github&utm_medium=readme
[Custom Readers]: https://arduinojson.org/v6/api/json/deserializejson/?utm_source=github&utm_medium=readme#custom-reader
[Custom Writers]: https://arduinojson.org/v6/api/json/serializejson/?utm_source=github&utm_medium=readme#custom-writer
[Custom Converters]: https://arduinojson.org/news/2021/05/04/version-6-18-0/?utm_source=github&utm_medium=readme
[Header-only Library]: https://en.wikipedia.org/wiki/Header-only

[Uno]: https://amzn.to/38aL2ik
[Due]: https://amzn.to/36YkWi2
[Micro]: https://amzn.to/35WkdwG
[Nano]: https://amzn.to/2QTvwRX
[Mega]: https://amzn.to/36XWhuf
[Yun]: https://amzn.to/30odURc
[Leonardo]: https://amzn.to/36XWjlR
[ESP8266]: https://amzn.to/36YluV8
[ESP32]: https://amzn.to/2G4pRCB
[D1 mini]: https://amzn.to/2QUpz7q
[D1 Mini Pro]: https://amzn.to/36UsGSs
[Teensy 4.0]: https://amzn.to/30ljXGq
[Teensy 3.2]: https://amzn.to/2FT0EuC
[Teensy 2.0]: https://amzn.to/2QXUMXj
[Argon]: https://amzn.to/2FQHa9X
[Boron]: https://amzn.to/36WgLUd
[Electron]: https://amzn.to/30vEc4k
[Photon]: https://amzn.to/387F9Cd
[MSP430]: https://amzn.to/30nJWgg

[Nios II]: https://en.wikipedia.org/wiki/Nios_II
[Arduino IDE]: https://www.arduino.cc/en/Main/Software
[Atmel Studio]: http://www.atmel.com/microsite/atmel-studio/
[Atollic TrueSTUDIO]: https://atollic.com/truestudio/
[Energia]: http://energia.nu/
[IAR]: https://www.iar.com/iar-embedded-workbench/
[Keil uVision]: http://www.keil.com/
[MPLAB X IDE]: http://www.microchip.com/mplab/mplab-x-ide
[Particle]: https://www.particle.io/
[PlatformIO]: http://platformio.org/
[Sloeber Plugin]: https://eclipse.baeyens.it/
[Visual Micro]: http://www.visualmicro.com/
[Visual Studio]: https://www.visualstudio.com/
[Wandbox]: https://wandbox.org/permlink/RlZSKy17DjJ6HcdN

[CMake Friendly]: https://arduinojson.org/v6/how-to/use-arduinojson-with-cmake/?utm_source=github&utm_medium=readme
[Elegant API]: http://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme
[Thread-safe]: https://en.wikipedia.org/wiki/Thread_safety
[`for` friendly]: https://arduinojson.org/v6/api/jsonobject/begin_end/?utm_source=github&utm_medium=readme
[TMP friendly]: https://en.wikipedia.org/wiki/Template_metaprogramming
[Integer Overflows]: https://arduinojson.org/v6/api/jsonvariant/as/?utm_source=github&utm_medium=readme#integer-overflows

[100% Coverage]: https://coveralls.io/github/bblanchon/ArduinoJson?branch=6.x
[VS Tests]: https://ci.appveyor.com/project/bblanchon/arduinojson/branch/6.x
[GCC Tests]: https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22
[Clang Tests]: https://github.com/bblanchon/ArduinoJson/actions?query=workflow%3A%22Continuous+Integration%22
[Continuous Fuzzing]: https://bugs.chromium.org/p/oss-fuzz/issues/list?sort=-opened&can=1&q=proj:arduinojson
[Clang-Tidy]: https://releases.llvm.org/10.0.0/tools/clang/tools/extra/docs/clang-tidy/

[Tutorials]: https://arduinojson.org/v6/doc/deserialization/?utm_source=github&utm_medium=readme
[Examples]: https://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme
[How-tos]: https://arduinojson.org/v6/example/?utm_source=github&utm_medium=readme
[FAQ]: https://arduinojson.org/v6/faq/?utm_source=github&utm_medium=readme
[Troubleshooter]: https://arduinojson.org/v6/troubleshooter/?utm_source=github&utm_medium=readme
[Book]: https://arduinojson.org/book/?utm_source=github&utm_medium=readme
[Changelog]: CHANGELOG.md
[GitHub]: https://github.com/search?o=desc&q=arduino+library&s=stars&type=Repositories
[Widely Used]: https://www.hackster.io/search?i=projects&q=arduinojson
[Responsive Support]: https://github.com/bblanchon/ArduinoJson/issues?q=is%3Aissue+is%3Aclosed
[Discord]: https://discord.gg/DzN6hHHD4h

[Tutorial Decoding]: https://arduinojson.org/doc/decoding/?utm_source=github&utm_medium=readme
[Tutorial Encoding]: https://arduinojson.org/doc/encoding/?utm_source=github&utm_medium=readme

[Become Sponsor]: https://github.com/sponsors/bblanchon
[Mastering ArduinoJson]: (https://arduinojson.org/book/
[Cast A Star]: https://github.com/bblanchon/ArduinoJson/stargazers
