Arduino JSON library
====================

[![Build status](https://ci.appveyor.com/api/projects/status/m7s53wav1l0abssg/branch/master?svg=true)](https://ci.appveyor.com/project/bblanchon/arduinojson/branch/master) [![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoJson) [![Coverage Status](https://img.shields.io/coveralls/bblanchon/ArduinoJson.svg)](https://coveralls.io/r/bblanchon/ArduinoJson?branch=master) 

*An elegant and efficient JSON library for embedded systems.*

It's designed to have the most intuitive API, the smallest footprint and works without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.

Features
--------

* JSON decoding (comments are supported)
* JSON encoding (with optional indentation)
* Elegant API, very easy to use
* Efficient (no malloc, nor copy)
* Portable (written in C++98)
* Self-contained (no external dependency)
* Small footprint
* MIT License

Works on
--------

* All Arduino boards
* ESP8266
* Teensy
* Computers (Windows, Linux, OSX...)

Quick start
-----------

#### Decoding / Parsing

```c++
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

const char* sensor = root["sensor"];
long time          = root["time"];
double latitude    = root["data"][0];
double longitude   = root["data"][1];
```

#### Encoding / Generating

```c++
StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.createObject();
root["sensor"] = "gps";
root["time"] = 1351824120;

JsonArray& data = root.createNestedArray("data");
data.add(48.756080, 6);  // 6 is the number of decimals to print
data.add(2.302038, 6);   // if not specified, 2 digits are printed

root.printTo(Serial);
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```


Documentation
-------------

The documentation is available online in the [Arduino JSON wiki](https://github.com/bblanchon/ArduinoJson/wiki)

Testimonials
------------

From Arduino's Forum user `jflaplante`:
> I tried aJson json-arduino before trying your library. I always ran into memory problem after a while.
> I have no such problem so far with your library. It is working perfectly with my web services.

From Arduino's Forum user `gbathree`:
> Thanks so much - this is an awesome library!  If you want to see what we're doing with it - the project is located at www.photosynq.org.

From StackOverflow user `thegreendroid`:
> It has a really elegant, simple API and it works like a charm on embedded and Windows/Linux platforms. We recently started using this on an embedded project and I can vouch for its quality.

From GitHub user `zacsketches`:

> Thanks for a great library!!!
> I've been watching you consistently develop this library over the past six months, and I used it today for a publish and subscribe architecture designed to help hobbyists move into more advanced robotics. Your library allowed me to implement remote subscription in order to facilitate multi-processor robots.
> ArduinoJson saved me a week's worth of time!!

[From Reddit user `erm_what_`](https://www.reddit.com/r/arduino/comments/3jj6ep/announcing_arduinojson_50/cusjk8c):

> This is a great library and I wouldn't be able to do the project I'm doing without it. I completely recommend it.

[From Reddit user `makerhacks`](https://www.reddit.com/r/arduino/comments/3jj6ep/announcing_arduinojson_50/cusqg7b):

> I am just starting an ESP8266 clock project and now I can output JSON from my server script and interpret it painlessly.

Donators
--------

Special thanks to the following persons and companies who made generous donations to the library author:

* Robert Murphy
* Surge Communications
* Alex Scott
* Firepick Services LLC
* A B Doodkorte
* Scott Smith

---

Found this library useful? Please star this project or [help me back with a donation!](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=donate%40benoitblanchon%2efr&lc=GB&item_name=Benoit%20Blanchon&item_number=Arduino%20JSON&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted) :smile:
