Arduino JSON library
====================

*A simple and efficient JSON library for embedded systems.*

It's design to be very lightweight, works without any allocation on the heap (no malloc).

It has been written with Arduino in mind, but it isn't linked to Arduino libraries so you can use this library on any other C++ project (in fact it's been written with Visual Studio ;-) 

# Features:

* JSON decoding: [more details here](/JsonParser/)
* JSON encoding: [more details here](/JsonGenerator/)
* Elegant API, very easy to use 
* Fixed memory allocation (no malloc)
* Small footprint
* MIT License

# Feature comparisons

| Library      | Memory allocation | Nested objects | Encoding | Code size |
| ------------ | ----------------- | -------------- | -------- | --------- |
| Arduino JSON | static | yes | yes |  2.7 kB |
| [json-arduino v1.5]( https://github.com/not404/json-arduino) | dynamic | no | no | 3.4 kB (+28%) |
| [aJson v1.0](https://github.com/interactive-matter/aJson) | dynamic | yes | yes | 5.2 kB (+94%) |

Code size was measured with a program parsing `{"sensor":"outdoor","value":25.6}`.
For each library, I wrote a program that extracts a string and a float. I subtracted the size of a program doing the same without any JSON parsing involved. The target platform was an Arduino Duemilanove.

# Testimonials

From Arduino's Forum user `jflaplante`:
> I tried the  [aJson and json-arduino] before trying your library. I always ran into memory problem after a while. 
> I have no such problem so far with your library. It is working perfectly with my web services.

From Arduino's Forum user `gbathree`:
> Thanks so much - this is an awesome library!  If you want to see what we're doing with it - the project is located at www.photosynq.org.

From StackOverflow user `thegreendroid`:
> It has a really elegant, simple API and it works like a charm on embedded and Windows/Linux platforms. We recently started using this on an embedded project and I can vouch for its quality.