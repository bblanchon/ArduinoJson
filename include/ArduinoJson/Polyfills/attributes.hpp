// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifdef _MSC_VER
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#else
#define FORCE_INLINE __attribute__((always_inline))
#define NO_INLINE __attribute__((noinline))
#endif
