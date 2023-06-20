// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>
#include <ArduinoJson/Variant/VariantTo.hpp>
#include "JsonVariantConst.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Grants access to the internal variant API
class VariantAttorney {
 public:
  template <typename TClient>
  FORCE_INLINE static ResourceManager* getResourceManager(TClient& client) {
    return client.getResourceManager();
  }

  template <typename TClient>
  FORCE_INLINE static auto getData(TClient& client)
      -> decltype(client.getData()) {
    return client.getData();
  }

  template <typename TClient>
  FORCE_INLINE static VariantData* getOrCreateData(TClient& client) {
    return client.getOrCreateData();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
