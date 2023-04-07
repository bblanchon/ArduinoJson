// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

// Include any header that might use the conflicting macros
#include <cmath>
#include <iostream>
#include <string>

// All cores
#define bit()
#define constrain()
#define DEFAULT
#define DISABLED
#define HIGH
#define INPUT
#define LOW
#define max()
#define min()
#define OUTPUT
#define round()
#define sq()
#define word()
#define bitRead()
#define bitSet()
#define bitClear()
#define bitWrite()
#define interrupts()
#define lowByte()
#define highByte()
#define DEC
#define HEX
#define OCT
#define BIN
#define cbi()
#define sbi()

// ESP8266
#define _max()
#define _min()

// Realtek Ameba
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define isprint(c)
#define isxdigit(c)
#define isspace(c)
#define isupper(c)
#define islower(c)
#define isalpha(c)

// issue #839
#define BLOCKSIZE
#define CAPACITY

// issue #1905
#define _current

// catch.hpp mutes several warnings, this file also allows to detect them
#include "ArduinoJson.h"
