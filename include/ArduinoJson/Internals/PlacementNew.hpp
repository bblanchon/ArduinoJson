// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifdef ARDUINO

// Declares the placement new as in <new>.
// This is required for Arduino IDE because it doesn't include the <new> header.
inline void *operator new(size_t, void *p) throw() { return p; }

#else

#include <new>

#endif
