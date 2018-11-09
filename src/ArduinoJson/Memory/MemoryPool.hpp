// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t

#include "../Polyfills/attributes.hpp"
#include "StringSlot.hpp"
#include "VariantSlot.hpp"

namespace ARDUINOJSON_NAMESPACE {

class MemoryPool {
 public:
  virtual StringSlot *allocExpandableString() = 0;
  virtual StringSlot *allocFrozenString(size_t) = 0;
  virtual StringSlot *expandString(StringSlot *) = 0;
  virtual void freezeString(StringSlot *, size_t) = 0;
  virtual void freeString(StringSlot *) = 0;

  virtual VariantSlot *allocVariant() = 0;
  virtual void freeVariant(VariantSlot *) = 0;

  virtual size_t size() const = 0;

 protected:
  // CAUTION: NO VIRTUAL DESTRUCTOR!
  // If we add a virtual constructor the Arduino compiler will add malloc()
  // and free() to the binary, adding 706 useless bytes.
  ~MemoryPool() {}
};
}  // namespace ARDUINOJSON_NAMESPACE
