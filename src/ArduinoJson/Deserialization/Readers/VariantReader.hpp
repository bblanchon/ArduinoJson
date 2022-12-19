// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/MemberProxy.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TVariant>
struct Reader<TVariant, typename enable_if<IsVariant<TVariant>::value>::type>
    : Reader<char*, void> {
  explicit Reader(const TVariant& x)
      : Reader<char*, void>(x.template as<const char*>()) {}
};

}  // namespace ARDUINOJSON_NAMESPACE
