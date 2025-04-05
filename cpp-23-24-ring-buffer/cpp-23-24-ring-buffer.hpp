#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
  std::vector<int> buffer;
  int head;
  int tail;
  int count;
  int capacity;

 public:
  explicit RingBuffer(size_t capacity)
      : capacity(capacity), head(0), tail(0), count(0), buffer(capacity) {}

  size_t Size() const { return count; }

  bool Empty() const { return count == 0; }

  bool TryPush(int value) {
    if (count >= capacity) return false;
    buffer[tail] = value;
    tail = (tail + 1) % capacity;
    count++;
    return true;
  }

  bool TryPop(int* value) {
    if (count == 0) return false;
    *value = buffer[head];
    head = (head + 1) % capacity;
    count--;
    return true;
  }
};