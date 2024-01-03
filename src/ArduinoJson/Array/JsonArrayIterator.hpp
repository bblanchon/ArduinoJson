// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/JsonVariant.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T>
class Ptr {
 public:
  Ptr(T value) : value_(value) {}

  T* operator->() {
    return &value_;
  }

  T& operator*() {
    return value_;
  }

 private:
  T value_;
};

class JsonArrayIterator {
  friend class JsonArray;

 public:
  JsonArrayIterator() {}
  explicit JsonArrayIterator(detail::ArrayData::iterator iterator,
                             detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  JsonVariant operator*() {
    return JsonVariant(iterator_.data(), resources_);
  }
  Ptr<JsonVariant> operator->() {
    return operator*();
  }

  bool operator==(const JsonArrayIterator& other) const {
    return iterator_ == other.iterator_;
  }

  bool operator!=(const JsonArrayIterator& other) const {
    return iterator_ != other.iterator_;
  }

  JsonArrayIterator& operator++() {
    iterator_.next(resources_);
    return *this;
  }

 private:
  detail::ArrayData::iterator iterator_;
  detail::ResourceManager* resources_;
};

class JsonArrayConstIterator {
  friend class JsonArray;

 public:
  JsonArrayConstIterator() {}
  explicit JsonArrayConstIterator(detail::ArrayData::iterator iterator,
                                  const detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  JsonVariantConst operator*() const {
    return JsonVariantConst(iterator_.data(), resources_);
  }
  Ptr<JsonVariantConst> operator->() {
    return operator*();
  }

  bool operator==(const JsonArrayConstIterator& other) const {
    return iterator_ == other.iterator_;
  }

  bool operator!=(const JsonArrayConstIterator& other) const {
    return iterator_ != other.iterator_;
  }

  JsonArrayConstIterator& operator++() {
    iterator_.next(resources_);
    return *this;
  }

 private:
  detail::ArrayData::iterator iterator_;
  const detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
