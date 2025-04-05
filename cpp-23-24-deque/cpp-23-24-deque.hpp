#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class Deque {
 private:
  static const size_t kInternalArraySize =
      (sizeof(T) < 64 ? 4096 / sizeof(T) : 64);

  size_t size_ = 0;
  size_t real_size_ = 0;
  size_t deque_size_ = 0;
  size_t deque_begin_ = 0;
  size_t deque_end_ = 0;
  T** external_array_ = nullptr;

  void swap(Deque& object);
  void copy(T**& new_external_array);
  void expand_external_array(const T& value);
  void allocate_new_block(const T& value);
  void allocate_new_block_front(const T& value);
  void expand_external_array_front(const T& value);
  void construct_element(T* ptr, const T& value);
  void destroy_elements() noexcept;
  void allocate_new_block_common(const T& value, size_t index, size_t offset);
  void expand_external_array_common(T** new_external_array, const T& value,
                                    size_t index, size_t offset);
  size_t divide_ceiling(size_t num1, size_t num2);

 public:
  template <bool IsConst>
  class CommonIterator {
   private:
    T** ptr_ = nullptr;
    std::ptrdiff_t position_ = 0;

   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    CommonIterator() noexcept = default;
    CommonIterator(T** ptr, size_t position) noexcept
        : ptr_(ptr), position_(position) {}
    CommonIterator(const CommonIterator& obj) noexcept = default;
    ~CommonIterator() noexcept = default;
    CommonIterator& operator=(const CommonIterator& obj) noexcept = default;

    CommonIterator& operator++() noexcept {
      if (++position_ == kInternalArraySize) {
        ++ptr_;
        position_ = 0;
      }
      return *this;
    }
    CommonIterator& operator--() noexcept {
      if (position_-- == 0) {
        --ptr_;
        position_ = kInternalArraySize - 1;
      }
      return *this;
    }
    CommonIterator operator++(int) noexcept {
      CommonIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    CommonIterator operator--(int) noexcept {
      CommonIterator tmp = *this;
      --(*this);
      return tmp;
    }

    CommonIterator& operator+=(difference_type number) noexcept {
      ptr_ += (number + position_) / kInternalArraySize;
      position_ = (number + position_) % kInternalArraySize;
      return *this;
    }
    CommonIterator& operator-=(difference_type number) noexcept {
      if (position_ < number) {
        ptr_ -= (number - position_ - 1) / kInternalArraySize + 1;
        position_ = kInternalArraySize -
                    ((number - position_ - 1) % kInternalArraySize) - 1;
      } else {
        position_ -= number;
      }
      return *this;
    }

    reference operator*() const noexcept { return *(*ptr_ + position_); }

    pointer operator->() const noexcept { return (*ptr_ + position_); }

    template <bool IsBool>
    difference_type operator-(
        const CommonIterator<IsBool>& iter) const noexcept {
      return (ptr_ - iter.ptr_) * kInternalArraySize +
             (position_ - iter.position_);
    }

    template <bool IsBool>
    bool operator<(const CommonIterator<IsBool>& iter) const noexcept {
      return (ptr_ < iter.ptr_) ||
             (ptr_ == iter.ptr_ && position_ < iter.position_);
    }

    template <bool IsBool>
    bool operator>(const CommonIterator<IsBool>& iter) const noexcept {
      return iter < *this;
    }

    template <bool IsBool>
    bool operator==(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this < iter) && !(iter < *this);
    }

    template <bool IsBool>
    bool operator!=(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this == iter);
    }

    template <bool IsBool>
    bool operator>=(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this < iter);
    }

    template <bool IsBool>
    bool operator<=(const CommonIterator<IsBool>& iter) const noexcept {
      return !(*this > iter);
    }

    CommonIterator operator+(size_t num) const noexcept {
      return CommonIterator(*this) += num;
    }

    CommonIterator operator-(size_t num) const noexcept {
      return CommonIterator(*this) -= num;
    }

    operator CommonIterator<true>() { return {ptr_, position_}; }
  };

  using reverse_iterator = std::reverse_iterator<CommonIterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<CommonIterator<true>>;
  using const_iterator = CommonIterator<true>;
  using iterator = CommonIterator<false>;

  Deque() noexcept;
  Deque(size_t count);
  Deque(size_t count, const T& value);
  Deque(const Deque& object);
  ~Deque() noexcept;
  Deque& operator=(const Deque& object);

  bool empty() const noexcept;
  size_t size() const noexcept;

  T& operator[](size_t index) noexcept;
  const T& operator[](size_t index) const noexcept;
  T& at(size_t index);
  const T& at(size_t index) const;

  void push_back(const T& value);
  void pop_back();
  void push_front(const T& value);
  void pop_front();

  iterator begin() noexcept;
  const_iterator begin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;

  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;

  const_reverse_iterator rcbegin() const noexcept;
  const_reverse_iterator rcend() const noexcept;

  void insert(iterator iter, const T& value);
  void erase(iterator iter);
};

//------------definitions_public--------------//

template <typename T>
Deque<T>::Deque() noexcept = default;

template <typename T>
Deque<T>::Deque(size_t count) : Deque(count, T()) {}

template <typename T>
Deque<T>::Deque(size_t count, const T& value)
    : size_(divide_ceiling(count, kInternalArraySize)),
      real_size_(size_),
      deque_size_(count),
      deque_begin_(0),
      deque_end_(deque_size_),
      external_array_(new T*[real_size_]) {
  for (size_t i = 0; i < size_; ++i) {
    external_array_[i] =
        reinterpret_cast<T*>(new uint8_t[sizeof(T) * kInternalArraySize]);
  }
  for (size_t i = deque_begin_; i < deque_end_; ++i) {
    size_t external_array_pos = i / kInternalArraySize;
    size_t internal_array_pos = i % kInternalArraySize;
    try {
      construct_element(
          external_array_[external_array_pos] + internal_array_pos, value);
    } catch (...) {
      destroy_elements();
      throw;
    }
  }
}

template <typename T>
Deque<T>::Deque(const Deque<T>& object)
    : size_(object.size_),
      real_size_(object.real_size_),
      deque_size_(object.deque_size_),
      deque_begin_(object.deque_begin_),
      deque_end_(object.deque_end_),
      external_array_(new T*[real_size_]) {
  for (size_t i = deque_begin_ / kInternalArraySize;
       i < divide_ceiling(deque_end_, kInternalArraySize); ++i) {
    external_array_[i] =
        reinterpret_cast<T*>(new uint8_t[sizeof(T) * kInternalArraySize]);
  }
  for (size_t i = deque_begin_; i < deque_end_; ++i) {
    size_t external_array_pos = i / kInternalArraySize;
    size_t internal_array_pos = i % kInternalArraySize;
    construct_element(
        external_array_[external_array_pos] + internal_array_pos,
        object.external_array_[external_array_pos][internal_array_pos]);
  }
}

template <typename T>
Deque<T>::~Deque() noexcept {
  destroy_elements();
}

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& object) {
  Deque<T> tmp = object;
  swap(tmp);
  return *this;
}

template <typename T>
bool Deque<T>::empty() const noexcept {
  return deque_size_ == 0;
}

template <typename T>
size_t Deque<T>::size() const noexcept {
  return deque_size_;
}

template <typename T>
T& Deque<T>::operator[](size_t index) noexcept {
  return external_array_[(deque_begin_ + index) / kInternalArraySize]
                        [(deque_begin_ + index) % kInternalArraySize];
}

template <typename T>
const T& Deque<T>::operator[](size_t index) const noexcept {
  return external_array_[(deque_begin_ + index) / kInternalArraySize]
                        [(deque_begin_ + index) % kInternalArraySize];
}

template <typename T>
T& Deque<T>::at(size_t index) {
  if (index < deque_size_) {
    return (*this)[index];
  }
  throw std::out_of_range("Index out of range");
}

template <typename T>
const T& Deque<T>::at(size_t index) const {
  if (index < deque_size_) {
    return (*this)[index];
  }
  throw std::out_of_range("Index out of range");
}

template <typename T>
void Deque<T>::push_front(const T& value) {
  if (real_size_ == 0) {
    *this = Deque<T>(1, value);
    return;
  }

  size_t index = deque_begin_ / kInternalArraySize;
  size_t offset = deque_begin_ % kInternalArraySize;

  if (deque_begin_ == 0) {
    expand_external_array_front(value);
  } else if (offset == 0) {
    allocate_new_block_front(value);
  } else {
    new (external_array_[index] + offset - 1) T(value);
    --deque_begin_;
  }

  ++deque_size_;
}

template <typename T>
void Deque<T>::pop_front() {
  if (empty()) {
    return;
  }

  size_t index = deque_begin_ / kInternalArraySize;
  size_t offset = deque_begin_ % kInternalArraySize;

  external_array_[index][offset].~T();

  if (offset == kInternalArraySize - 1) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[index]);
    --size_;
  }

  ++deque_begin_;
  --deque_size_;
}

template <typename T>
void Deque<T>::pop_back() {
  if (empty()) {
    return;
  }

  size_t deque_end_minus_one = deque_end_ - 1;
  size_t external_array_pos = deque_end_minus_one / kInternalArraySize;
  size_t internal_array_pos = deque_end_minus_one % kInternalArraySize;

  external_array_[external_array_pos][internal_array_pos].~T();

  if (internal_array_pos == 0) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[external_array_pos]);
    --size_;
  }

  --deque_end_;
  --deque_size_;
}

template <typename T>
void Deque<T>::push_back(const T& value) {
  if (real_size_ == 0) {
    *this = Deque<T>(1, value);
    return;
  }

  size_t index = deque_end_ / kInternalArraySize;
  size_t offset = deque_end_ % kInternalArraySize;

  if (index == real_size_) {
    expand_external_array(value);
  } else if (offset == 0) {
    allocate_new_block(value);
  } else {
    new (external_array_[index] + offset) T(value);
    ++deque_end_;
  }

  ++deque_size_;
}

template <typename T>
typename Deque<T>::iterator Deque<T>::begin() noexcept {
  return CommonIterator<false>(
      external_array_ + deque_begin_ / kInternalArraySize,
      deque_begin_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::begin() const noexcept {
  return CommonIterator<true>(
      external_array_ + deque_begin_ / kInternalArraySize,
      deque_begin_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::end() const noexcept {
  return CommonIterator<true>(external_array_ + deque_end_ / kInternalArraySize,
                              deque_end_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::iterator Deque<T>::end() noexcept {
  return CommonIterator<false>(
      external_array_ + deque_end_ / kInternalArraySize,
      deque_end_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const noexcept {
  return CommonIterator<true>(
      external_array_ + deque_begin_ / kInternalArraySize,
      deque_begin_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const noexcept {
  return CommonIterator<true>(external_array_ + deque_end_ / kInternalArraySize,
                              deque_end_ % kInternalArraySize);
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rcbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rcend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T>
void Deque<T>::insert(iterator iter, const T& value) {
  size_t pos = iter - begin();
  push_back(value);
  std::rotate(begin() + pos, end() - 1, end());
}

template <typename T>
void Deque<T>::erase(iterator iter) {
  size_t pos = iter - begin();
  std::rotate(begin() + pos, begin() + pos + 1, end());
  pop_back();
}

//------------definitions_private--------------//

template <typename T>
void Deque<T>::swap(Deque<T>& object) {
  std::swap(size_, object.size_);
  std::swap(real_size_, object.real_size_);
  std::swap(deque_size_, object.deque_size_);
  std::swap(deque_begin_, object.deque_begin_);
  std::swap(deque_end_, object.deque_end_);
  std::swap(external_array_, object.external_array_);
}

template <typename T>
void Deque<T>::copy(T**& new_external_array) {
  size_t begin = 3 * divide_ceiling(real_size_, 2);
  size_t end = 3 * divide_ceiling(real_size_, 2) + size_;
  for (size_t i = begin; i < end; ++i) {
    new_external_array[i] =
        external_array_[i - begin + deque_begin_ / kInternalArraySize];
  }
}

template <typename T>
void Deque<T>::expand_external_array_front(const T& value) {
  const size_t kSix = 6;
  T** new_external_array = new T*[size_ + kSix * divide_ceiling(real_size_, 2)];
  copy(new_external_array);
  size_t begin = 3 * divide_ceiling(real_size_, 2) - 1;
  expand_external_array_common(new_external_array, value, begin,
                               kInternalArraySize - 1);
  deque_begin_ = (3 * divide_ceiling(real_size_, 2) - 1) * kInternalArraySize +
                 kInternalArraySize - 1;
  deque_end_ =
      (3 * divide_ceiling(real_size_, 2) + size_ - 1) * kInternalArraySize +
      (deque_end_ % kInternalArraySize) +
      kInternalArraySize * (int)(deque_end_ % kInternalArraySize == 0);
  real_size_ = kSix * divide_ceiling(real_size_, 2) + size_;
  ++size_;
}

template <typename T>
void Deque<T>::expand_external_array(const T& value) {
  const size_t kSix = 6;
  T** new_external_array = new T*[size_ + kSix * divide_ceiling(real_size_, 2)];
  copy(new_external_array);
  size_t end = 3 * divide_ceiling(real_size_, 2) + size_ + 1;
  expand_external_array_common(new_external_array, value, end - 1, 0);
  deque_begin_ = (3 * divide_ceiling(real_size_, 2)) * kInternalArraySize +
                 deque_begin_ % kInternalArraySize;
  deque_end_ =
      (3 * divide_ceiling(real_size_, 2) + size_) * kInternalArraySize + 1;
  real_size_ = kSix * divide_ceiling(real_size_, 2) + size_;
  ++size_;
}

template <typename T>
void Deque<T>::allocate_new_block_front(const T& value) {
  allocate_new_block_common(value, deque_begin_ / kInternalArraySize - 1,
                            kInternalArraySize - 1);
  --deque_begin_;
  ++size_;
}

template <typename T>
void Deque<T>::allocate_new_block(const T& value) {
  allocate_new_block_common(value, deque_end_ / kInternalArraySize, 0);
  ++deque_end_;
  ++size_;
}
template <typename T>
void Deque<T>::allocate_new_block_common(const T& value, size_t index,
                                         size_t offset) {
  external_array_[index] =
      reinterpret_cast<T*>(new uint8_t[sizeof(T) * kInternalArraySize]);
  try {
    new (external_array_[index] + offset) T(value);
  } catch (...) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[index]);
    throw;
  }
}

template <typename T>
void Deque<T>::expand_external_array_common(T** new_external_array,
                                            const T& value, size_t index,
                                            size_t offset) {
  new_external_array[index] =
      reinterpret_cast<T*>(new uint8_t[sizeof(T) * kInternalArraySize]);
  try {
    new (new_external_array[index] + offset) T(value);
  } catch (...) {
    delete[] reinterpret_cast<uint8_t*>(new_external_array[index]);
    delete[] new_external_array;
    throw;
  }
  delete[] external_array_;
  external_array_ = new_external_array;
}

template <typename T>
void Deque<T>::construct_element(T* ptr, const T& value) {
  try {
    new (ptr) T(value);
  } catch (...) {
    ptr->~T();
    throw;
  }
}

template <typename T>
void Deque<T>::destroy_elements() noexcept {
  for (size_t i = deque_begin_; i < deque_end_; ++i) {
    size_t external_array_pos = i / kInternalArraySize;
    size_t internal_array_pos = i % kInternalArraySize;
    (external_array_[external_array_pos] + internal_array_pos)->~T();
  }
  for (size_t i = deque_begin_ / kInternalArraySize;
       i < divide_ceiling(deque_end_, kInternalArraySize); ++i) {
    delete[] reinterpret_cast<uint8_t*>(external_array_[i]);
  }
  delete[] external_array_;
}
template <typename T>
size_t Deque<T>::divide_ceiling(size_t num1, size_t num2) {
  return static_cast<size_t>(ceil((double)num1 / (double)num2));
}