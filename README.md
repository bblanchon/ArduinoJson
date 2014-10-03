Arduino JSON library
====================

*An elegant and efficient JSON library for embedded systems.*

It's design to have the most intuitive API, the smallest footprint and works without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library in any other C++ project.

Features
--------

* JSON decoding: [see documentation here](/JsonParser/)
* JSON encoding: [see documentation here](/JsonGenerator/)
* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* MIT License

Feature comparison
------------------

| Library      | Memory allocation | Nested objects | Parser size | Encoder size  |
| ------------ | ----------------- | -------------- | ----------- | ------------- |
| Arduino JSON | static            | yes            | 2760 Bytes  | 862 bytes     |
| json-arduino | dynamic           | no             | 3348 (+21%) | not supported |
| aJson        | dynamic           | yes            | 5088 (+84%) | 4678 (+540%)  |

"Parser size" was measured with a program parsing `{"sensor":"outdoor","value":25.6}`.
For each library, I wrote a program that extracts a string and a float. I subtracted the size of a program doing the same without any JSON parsing involved. [Source files are here](https://gist.github.com/bblanchon/e8ba914a7109f3642c0f).

"Encoder size" was measured with a program generating `{"sensor":"outdoor","value":25.6}`.
[Source files are here](https://gist.github.com/bblanchon/60224e9dcfeab4ddc7e9).

In each case the target platform was an Arduino Duemilanove and Arduino IDE 1.0.5 was used. 

Links: [json-arduino](https://github.com/not404/json-arduino), [aJson](https://github.com/interactive-matter/aJson) 

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

Related blog posts
-----

* [The project I originally wrote this library for](http://blog.benoitblanchon.fr/rfid-payment-terminal/)
* [Motivation for this library](http://blog.benoitblanchon.fr/arduino-json-parser/)
* [Release of version 2](http://blog.benoitblanchon.fr/arduino-json-v2-0/)
* [Release of version 3](http://blog.benoitblanchon.fr/arduino-json-v3-0/)