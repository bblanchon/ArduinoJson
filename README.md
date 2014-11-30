Arduino JSON library
====================

[![Build Status](https://travis-ci.org/bblanchon/ArduinoJson.svg?branch=master)](https://travis-ci.org/bblanchon/ArduinoJson) [![Coverage Status](https://img.shields.io/coveralls/bblanchon/ArduinoJson.svg)](https://coveralls.io/r/bblanchon/ArduinoJson?branch=master)

*An elegant and efficient JSON library for embedded systems.*

It's design to have the most intuitive API, the smallest footprint and works without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.

Features
--------

* JSON decoding
* JSON encoding (with optional indentation)
* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* MIT License

Quick start
-----------

#### Decoding / Parsing
   
    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(json);

    const char* sensor = root["sensor"];
    long time          = root["time"];
    double latitude    = root["data"][0];
    double longitude   = root["data"][1];

#### Encoding / Generating
   
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

---

Found this library useful? [Help me back with a donation!](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=donate%40benoitblanchon%2efr&lc=GB&item_name=Benoit%20Blanchon&item_number=Arduino%20JSON&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted) :smile: