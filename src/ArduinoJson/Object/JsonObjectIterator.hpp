// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Object/JsonPair.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

class JsonObjectIterator {
  friend class JsonObject;

 public:
  JsonObjectIterator() {}

  explicit JsonObjectIterator(detail::ObjectData::iterator iterator,
                              detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  JsonPair operator*() const {
    return JsonPair(iterator_, resources_);
  }
  Ptr<JsonPair> operator->() {
    return operator*();
  }

  bool operator==(const JsonObjectIterator& other) const {
    return iterator_ == other.iterator_;
  }

  bool operator!=(const JsonObjectIterator& other) const {
    return iterator_ != other.iterator_;
  }

  JsonObjectIterator& operator++() {
    iterator_.next(resources_);  // key
    iterator_.next(resources_);  // value
    return *this;
  }

 private:
  detail::ObjectData::iterator iterator_;
  detail::ResourceManager* resources_;
};

class JsonObjectConstIterator {
  friend class JsonObject;

 public:
  JsonObjectConstIterator() {}

  explicit JsonObjectConstIterator(detail::ObjectData::iterator iterator,
                                   const detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  JsonPairConst operator*() const {
    return JsonPairConst(iterator_, resources_);
  }
  Ptr<JsonPairConst> operator->() {
    return operator*();
  }

  bool operator==(const JsonObjectConstIterator& other) const {
    return iterator_ == other.iterator_;
  }

  bool operator!=(const JsonObjectConstIterator& other) const {
    return iterator_ != other.iterator_;
  }

  JsonObjectConstIterator& operator++() {
    iterator_.next(resources_);  // key
    iterator_.next(resources_);  // value
    return *this;
  }

 private:
  detail::ObjectData::iterator iterator_;
  const detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
