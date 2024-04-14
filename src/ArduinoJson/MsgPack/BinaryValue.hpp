#pragma once

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

template <typename T>
class BinaryValue {
 public:
  explicit BinaryValue(const T& c) : container_(c) {}
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

template <typename T>
class BinaryValue<T*> {
 public:
  explicit BinaryValue(const T* c, size_t size) : data_(c), size_(size) {}
  operator const T*() const {
    return data_;
  }

  const T* begin() const {
    return data_;
  }

  const T* end() const {
    return data_ + size_;
  }

  size_t size_bytes() const {
    return size_ * sizeof(T);
  }

 private:
  const T* data_;
  size_t size_;
};

template <>
class BinaryValue<detail::StringNode*> {
 public:
  explicit BinaryValue(detail::StringNode* stringNode)
      : stringNode_(stringNode) {}
  operator const void*() const {
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

using Binary = BinaryValue<detail::StringNode*>;

template <typename T>
inline BinaryValue<T> binary(const T& value) {
  return BinaryValue<T>(value);
}

template <typename T>
inline BinaryValue<T*> binary(const T* value, size_t size) {
  return BinaryValue<T*>(value, size);
}

ARDUINOJSON_END_PUBLIC_NAMESPACE
