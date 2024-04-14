#pragma once

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T>
class OwnedBinaryValue {
 public:
  explicit OwnedBinaryValue(const T& c) : container_(c) {}
  operator const T&() const {
    return container_;
  }

  typename T::const_iterator begin() const {
    return container_.cbegin();
  }

  typename T::const_iterator end() const {
    return container_.cend();
  }

  size_t size_bytes() const {
    return sizeof(typename T::value_type) * container_.size();
  }

 private:
  const T& container_;
};

template <>
class OwnedBinaryValue<detail::StringNode*> {
 public:

  explicit OwnedBinaryValue(detail::StringNode* stringNode)
      : stringNode_(stringNode) {}
  operator void*() const {
    return stringNode_->data;
  }

  void* data() const {
    return stringNode_->data;
  }

  size_t size_bytes() const {
    return stringNode_->length;
  }

  detail::StringNode* string_node() const {
    return stringNode_;
  }

 private:
  detail::StringNode* stringNode_;
};

class LinkedBinaryValue {
 public:
  explicit LinkedBinaryValue(const void* p, size_t n) : data_(p), size_(n) {}
  operator const void*() const {
    return data_;
  }

  const void* data() const {
    return data_;
  }

  size_t size_bytes() const {
    return size_;
  }

 private:
  const void* data_;
  size_t size_;
};

using BinaryValue = LinkedBinaryValue;

template <typename T>
inline OwnedBinaryValue<T> binary(const T& value) {
  return OwnedBinaryValue<T>(value);
}

template <typename T>
inline LinkedBinaryValue binary(const T* value, size_t size) {
  return LinkedBinaryValue(value, size);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
