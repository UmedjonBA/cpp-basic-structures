#include <gtest/gtest.h>
#include "cpp-23-24-smart_pointers.hpp"
#include "utils.hpp"
#include "memory_utils.hpp"

size_t MemoryManager::type_new_allocated = 0;
size_t MemoryManager::type_new_deleted = 0;
size_t MemoryManager::allocator_allocated = 0;
size_t MemoryManager::allocator_deallocated = 0;
size_t MemoryManager::allocator_constructed = 0;
size_t MemoryManager::allocator_destroyed = 0;

template <typename T, bool PropagateOnConstruct, bool PropagateOnAssign>
size_t
    WhimsicalAllocator<T, PropagateOnConstruct, PropagateOnAssign>::counter = 0;

size_t Accountant::ctor_calls = 0;
size_t Accountant::dtor_calls = 0;

size_t Base::base_created = 0;
size_t Base::base_destroyed = 0;
size_t Derived::derived_created = 0;
size_t Derived::derived_destroyed = 0;

bool ThrowingAccountant::need_throw = false;

void SetupTest() {
  MemoryManager::type_new_allocated = 0;
  MemoryManager::type_new_deleted = 0;
  MemoryManager::allocator_allocated = 0;
  MemoryManager::allocator_deallocated = 0;
  MemoryManager::allocator_constructed = 0;
  MemoryManager::allocator_destroyed = 0;
  Accountant::reset();
  Base::reset();
  Derived::reset();
  NonVirtualDerived::reset();
}


TEST(SharedPtr, Constructors) {
  {
    SharedPtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
  }

  {
    SharedPtr<int> ptr(nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
  }

  {
    int* raw = new int(10);
    SharedPtr<int> ptr(raw);
    EXPECT_EQ(ptr.get(), raw);
    EXPECT_EQ(ptr.use_count(), 1);
  }

  {
    int* raw = new int(10);
    SharedPtr<int> ptr(raw);
    EXPECT_EQ(ptr.get(), raw);

    SharedPtr<int> ptr2(std::move(ptr));
    EXPECT_EQ(ptr2.get(), raw);
    EXPECT_EQ(ptr2.use_count(), 1);

    EXPECT_EQ(ptr.get(), nullptr);
  }
}

TEST(SharedPtr, Assignment) { // NOLINT
  SharedPtr<int> a;
  SharedPtr<int> b;

  auto ptr = new int(11);
  { // value assignment
    a = SharedPtr<int>(ptr);
    EXPECT_EQ(a.get(), ptr);
  }

  ptr = new int(13);
  { // reassigning
    a = SharedPtr<int>(ptr);
    EXPECT_EQ(a.get(), ptr);
  }

  { // copy assignment
    b = a;
    EXPECT_EQ(b.get(), ptr);
    EXPECT_EQ(a.get(), ptr);
  }

  { // copy is independent
    b = SharedPtr<int>(nullptr);
    EXPECT_EQ(a.get(), ptr);
    EXPECT_EQ(b.get(), nullptr);
  }

  { // move
    b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(b.get(), ptr);
  }

  { // self-assignment
    b = b;
    EXPECT_EQ(b.get(), ptr);
  }
}

TEST(SharedPtr, UseCount) { // NOLINT
  SharedPtr<int> a;
  SharedPtr<int> b(new int(6));

  EXPECT_EQ(a.use_count(), 0);
  EXPECT_EQ(b.use_count(), 1);

  a = b;
  EXPECT_EQ(a.use_count(), 2);
  EXPECT_EQ(b.use_count(), 2);

  { // copy/move
    SharedPtr<int> c(a);
    EXPECT_EQ(a.use_count(), 3);
    EXPECT_EQ(b.use_count(), 3);
    EXPECT_EQ(c.use_count(), 3);

    SharedPtr<int> d(std::move(c));
    EXPECT_EQ(a.use_count(), 3);
    EXPECT_EQ(b.use_count(), 3);
    EXPECT_EQ(c.use_count(), 0);
    EXPECT_EQ(d.use_count(), 3);
  }

  EXPECT_EQ(a.use_count(), 2);
  EXPECT_EQ(b.use_count(), 2);

  a = nullptr;
  EXPECT_EQ(a.use_count(), 0);
  EXPECT_EQ(b.use_count(), 1);
}

TEST(SharedPtr, Operators) { // NOLINT
  { // operator*
    SharedPtr<int> a(new int(19));
    EXPECT_EQ(*a, 19);
  }

  { // operator->
    auto ptr = new int(11);
    SharedPtr<SharedPtr<int>> a(new SharedPtr<int>(ptr));
    EXPECT_EQ(a->use_count(), 1);
    EXPECT_EQ(a->get(), ptr);
  }
}

TEST(SharedPtr, Const) {
  const SharedPtr<int> ptr(new int(10));
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_EQ(*ptr.get(), 10);
  EXPECT_EQ(*ptr, 10);
}

TEST(MakeShared, Basic) {
  SetupTest();

  auto ptr = MakeShared<OnlyMovable>(OnlyMovable(10));
  WeakPtr<OnlyMovable> weak = ptr;
  EXPECT_FALSE(weak.expired());
  {
    auto copy_ptr = ptr;
    EXPECT_EQ(copy_ptr.use_count(), 2);
  }
  EXPECT_FALSE(weak.expired());
  ptr.reset();
  EXPECT_TRUE(weak.expired());
}

TEST(MakeShared, Accountant) {
  SetupTest();

  auto ptr = MakeShared<Accountant>();
  EXPECT_EQ(Accountant::ctor_calls, 1);

  WeakPtr<Accountant> weak = ptr;
  auto copy_ptr = ptr;
  ptr.reset();
  EXPECT_EQ(Accountant::ctor_calls, 1);
  EXPECT_EQ(Accountant::dtor_calls, 0);
  EXPECT_FALSE(weak.expired());

  copy_ptr.reset();
  EXPECT_TRUE(weak.expired());
  EXPECT_EQ(Accountant::ctor_calls, 1);
  EXPECT_EQ(Accountant::dtor_calls, 1);
}

TEST(Shared, Inheritance) {
  SetupTest();

  {
    SharedPtr<Derived> derived(new Derived());
    SharedPtr<Base> base(new Base());
    base = derived;
  }

  EXPECT_EQ(Derived::derived_created, 1);
  EXPECT_EQ(Derived::derived_destroyed, 1);
  EXPECT_EQ(Base::base_created, 2);
  EXPECT_EQ(Base::base_destroyed, 2);

}

TEST(Shared, Inheritance2) {
  SetupTest();
  {
    auto ptr = new NonVirtualDerived();
    SharedPtr<NonVirtualBase> shared(ptr);
  }
  EXPECT_EQ(NonVirtualDerived::created, 1);
  EXPECT_EQ(NonVirtualDerived::destroyed, 1);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}