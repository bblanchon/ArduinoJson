// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantImpl.hpp>
#include "JsonVariantConst.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Grants access to the internal variant API
class VariantAttorney {
 public:
  template <typename TClient>
  static auto getResourceManager(TClient& client)
      -> decltype(client.getResourceManager()) {
    return client.getResourceManager();
  }

  template <typename TClient>
  static auto getData(TClient& client) -> decltype(client.getData()) {
    return client.getData();
  }

  template <typename TClient>
  static VariantImpl getVariantImpl(TClient& client) {
    return VariantImpl(client.getData(), client.getResourceManager());
  }

  template <typename TClient>
  static VariantImpl getOrCreateVariantImpl(TClient& client) {
    return VariantImpl(client.getOrCreateData(), client.getResourceManager());
  }

  template <typename TClient>
  static VariantData* getOrCreateData(TClient& client) {
    return client.getOrCreateData();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
