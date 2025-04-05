#pragma once
#include <iostream>
#include <vector>

class String {
 private:
  char* arr;
  size_t capacity{0};
  size_t size{0};
  int strlen(const char* string);
  void MemoryRedefinitions(int new_capacity);
  size_t Find(const String& delim, size_t begin);
  String Substr(size_t start, size_t end);

 public:
  String();
  String(const size_t i);
  String(const size_t i, char character);
  String(const char* string);
  String(const String& string);
  ~String();
  String& operator=(const String& string);
  String operator+(const String& string) const;
  String& operator+=(const String& string);
  String& operator*=(int x);
  String operator*(int i) const;
  bool operator>(const String& string) const;
  bool operator<(const String& string) const;
  bool operator>=(const String& string) const;
  bool operator<=(const String& string) const;
  bool operator==(const String& string) const;
  bool operator!=(const String& string) const;
  char& operator[](int i);
  const char& operator[](int i) const;
  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  char& Front();
  char& Back();
  const char& Front() const;
  const char& Back() const;
  bool Empty() const;
  const char* Data() const;
  size_t Size() const;
  size_t Capacity() const;
  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings);
  friend std::ostream& operator<<(std::ostream& out, const String& res);
  friend std::istream& operator>>(std::istream& in, String& res);
};
