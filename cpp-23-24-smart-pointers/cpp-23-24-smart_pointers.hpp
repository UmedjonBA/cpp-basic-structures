#include <iostream>
#include <typeinfo>
#include <utility>

template <typename T>
class WeakPtr;
struct ControlBlockBase {
  int shared_count;
  int weak_count;

  ControlBlockBase() : shared_count(1), weak_count(0) {}

  virtual ~ControlBlockBase() = default;
  virtual bool get_creation_method() { return false; }
  ControlBlockBase* operator->() { return this; }
  virtual void clear() = 0;
};

template <typename T>
struct ControlBlock : public ControlBlockBase {
  T obj_ptr;

  ControlBlock(T& ptr) : obj_ptr(ptr) {}
  ControlBlock(T&& ptr) : obj_ptr(std::move(ptr)) {}
  template <typename... Args>
  ControlBlock(Args&&... args) : obj_ptr(std::forward<Args>(args)...) {}
  bool get_creation_method() override { return true; }
  T* get_ptr() { return &obj_ptr; }
  ControlBlock* operator->() { return this; }
  void clear() override { obj_ptr.~T(); }
};
template <typename T>
struct ControlBlockPTR : public ControlBlockBase {
  T* obj_ptr;

  ControlBlockPTR(T* ptr) : obj_ptr(ptr) {}

  T* get_ptr() { return obj_ptr; }

  ControlBlockPTR* operator->() { return this; }
  ~ControlBlockPTR() override { clear(); }
  void clear() override { delete obj_ptr; }
};

template <typename T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr), controlBlock_(nullptr) {}
  SharedPtr(std::nullptr_t) : ptr_(nullptr), controlBlock_(nullptr) {}

  template <typename Y>
  explicit SharedPtr(Y* ptr) {
    controlBlock_ = new ControlBlockPTR<Y>(ptr);
    this->ptr_ = ptr;
  }

  SharedPtr(const SharedPtr& other) {
    ptr_ = other.ptr_;
    controlBlock_ = other.controlBlock_;
    if (controlBlock_ != nullptr) {
      controlBlock_->shared_count++;
    }
  }
  SharedPtr(SharedPtr&& other) noexcept
      : ptr_(other.ptr_), controlBlock_(other.controlBlock_) {
    other.controlBlock_ = nullptr;
    other.ptr_ = nullptr;
  }
  template <typename U>
  SharedPtr(const SharedPtr<U>& other) {
    ptr_ = other.ptr_;
    controlBlock_ = other.controlBlock_;
    if (controlBlock_ != nullptr) {
      controlBlock_->shared_count++;
    }
  }
  template <typename U>
  SharedPtr(SharedPtr<U>&& other) noexcept
      : ptr_(other.ptr_), controlBlock_(other.controlBlock_) {
    other.controlBlock_ = nullptr;
    other.ptr_ = nullptr;
  }

  ~SharedPtr() {
    if (controlBlock_ != nullptr) {
      if (controlBlock_->shared_count > 0) {
        controlBlock_->shared_count--;
      }
      if (controlBlock_->shared_count == 0 && controlBlock_->weak_count == 0) {
        delete controlBlock_;
      } else if (controlBlock_->shared_count == 0 &&
                 controlBlock_->weak_count != 0) {
        controlBlock_->clear();
      }
    }
  }

  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      reset();
      ptr_ = other.ptr_;
      controlBlock_ = other.controlBlock_;
      if (controlBlock_ != nullptr) {
        controlBlock_->shared_count++;
      }
    }
    return *this;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      reset();
      ptr_ = other.ptr_;
      controlBlock_ = other.controlBlock_;
      other.controlBlock_ = nullptr;
      other.ptr_ = nullptr;
    }
    return *this;
  }
  template <typename U>
  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      reset();
      ptr_ = other.ptr;
      controlBlock_ = other.controlBlock_;
      if (controlBlock_ != nullptr) {
        controlBlock_->shared_count++;
      }
    }
    return *this;
  }
  template <typename U>
  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      reset();
      ptr_ = other.ptr;
      controlBlock_ = other.controlBlock_;
      other.controlBlock_ = nullptr;
      other.ptr = nullptr;
    }
    return *this;
  }

  template <typename U, typename... Args>
  friend SharedPtr<U> MakeShared(Args&&... args);

  T& operator*() const { return *ptr_; }

  T* operator->() const { return ptr_; }

  int use_count() const {
    return (controlBlock_ != nullptr) ? controlBlock_->shared_count : 0;
  }
  void reset() {
    if (controlBlock_ != nullptr) {
      controlBlock_->shared_count--;
      if (controlBlock_->shared_count == 0) {
        this->~SharedPtr();
      }
    }
    ptr_ = nullptr;
    controlBlock_ = nullptr;
  }
  T* get() const { return ptr_; }

 private:
  T* ptr_;
  ControlBlockBase* controlBlock_;

  SharedPtr(ControlBlock<T>* control_block)
      : controlBlock_(control_block), ptr_(control_block->get_ptr()) {}

  template <typename Y>
  friend class SharedPtr;

  friend class WeakPtr<T>;
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new ControlBlock<T>(std::forward<Args>(args)...));
}

template <typename T>
class WeakPtr {
 public:
  WeakPtr() : ptr_(nullptr), controlBlock_(nullptr) {}

  WeakPtr(const WeakPtr& other) {
    ptr_ = other.ptr_;
    controlBlock_ = other.controlBlock_;
    if (controlBlock_ != nullptr) {
      controlBlock_->weak_count++;
    }
  }

  WeakPtr(const SharedPtr<T>& shared) {
    ptr_ = shared.ptr_;
    controlBlock_ = shared.controlBlock_;
    if (controlBlock_ != nullptr) {
      controlBlock_->weak_count++;
    }
  }

  WeakPtr(WeakPtr&& other) noexcept {
    ptr_ = other.ptr_;
    controlBlock_ = other.controlBlock_;
    other.ptr_ = nullptr;
    other.controlBlock_ = nullptr;
  }

  ~WeakPtr() {
    if (controlBlock_ != nullptr) {
      if (controlBlock_->weak_count > 0) {
        controlBlock_->weak_count--;
      }
      if (controlBlock_->shared_count == 0 && controlBlock_->weak_count == 0) {
        if (!controlBlock_->get_creation_method()) {
          delete ptr_;
        }
        delete controlBlock_;
      }
    }
  }

  WeakPtr& operator=(const WeakPtr& other) {
    if (this != &other) {
      this->~WeakPtr();
      this = WeakPtr<T>(other);
    }
    return *this;
  }

  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (this != &other) {
      this->~WeakPtr();
      *this = WeakPtr<T>(std::move(other));
    }
    return *this;
  }

  bool expired() const {
    return controlBlock_ != nullptr && controlBlock_->shared_count == 0;
  }

  SharedPtr<T> lock() const {
    auto temp = SharedPtr<T>();
    if (!expired()) {
      return temp;
    }
    temp.ptr = ptr_;
    temp.controlBlock_ = controlBlock_;
    return temp;
  }

 private:
  T* ptr_;
  ControlBlockBase* controlBlock_;

  template <typename Y>
  friend class SharedPtr;
};
