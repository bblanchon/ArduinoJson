// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#ifdef ARDUINO

inline void *operator new(size_t, void *p) throw() { return p; }

#else

#include <new>

#endif
