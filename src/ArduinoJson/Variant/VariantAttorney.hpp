// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>
#include "JsonVariantConst.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Grants access to the internal variant API
class VariantAttorney {
  // Tells whether getData() returns a const pointer
  template <typename TClient>
  struct ResultOfGetData {
   protected:  // <- to avoid GCC's "all member functions in class are private"
    static int probe(const VariantData*);
    static char probe(VariantData*);

    static TClient& client;

   public:
    typedef typename conditional<sizeof(probe(client.getData())) == sizeof(int),
                                 const VariantData*, VariantData*>::type type;
  };

 public:
  template <typename TClient>
  FORCE_INLINE static MemoryPool* getPool(TClient& client) {
    return client.getPool();
  }

  template <typename TClient>
  FORCE_INLINE static typename ResultOfGetData<TClient>::type getData(
      TClient& client) {
    return client.getData();
  }

  template <typename TClient>
  FORCE_INLINE static VariantData* getOrCreateData(TClient& client) {
    return client.getOrCreateData();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
