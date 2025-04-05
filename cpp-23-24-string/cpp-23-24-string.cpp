#include "cpp-23-24-string.hpp"

int String::strlen(const char* string) {
  if (string == nullptr) return 0;
  int counter = 0;
  while (string[counter] != '\0') {
    counter++;
  }
  return counter;
}

void String::MemoryRedefinitions(int new_capacity) {
  char* res = new char[new_capacity];
  capacity = new_capacity;
  for (size_t i = 0; i < size; ++i) res[i] = arr[i];
  if (size != 0) delete[] arr;
  arr = res;
}

std::ostream& operator<<(std::ostream& out, const String& res) {
  out << res.arr;
  return out;
}

std::istream& operator>>(std::istream& in, String& res) {
  if (res.size != 0) delete[] res.arr;
  res.arr = NULL;
  res.size = 0;
  res.capacity = 0;
  char temp[2];  // Это не магическая константа, просто нужна стрка длиной 1
                 // симвои и '\0' чтобы смочь его добавить в свой String
  in >> temp[0];
  temp[1] = '\0';
  while (temp[0] != ' ' && temp[0] != '\n' && !in.eof()) {
    res += temp;
    in >> temp[0];
  }
  return in;
}

String::String() : capacity(0), size(0), arr(nullptr) {}

String::String(const size_t i) : capacity(i), size(i) {
  arr = new char[capacity + 1];
}

String::String(const size_t i, char character)
    : capacity(i), size(i), arr(new char[i + 1]) {
  for (size_t j = 0; j < i; ++j) arr[j] = character;
  arr[i] = '\0';
}

String::String(const char* string)
    : capacity(strlen(string)),
      size(strlen(string)),
      arr(new char[strlen(string) + 1]) {
  for (size_t i = 0; i <= size; ++i) arr[i] = string[i];
}

String::String(const String& string)
    : size(string.size),
      capacity(string.capacity),
      arr(new char[string.capacity + 1]) {
  if (string.arr != nullptr) {
    for (size_t i = 0; i <= size; ++i) arr[i] = string.arr[i];
  } else {
    delete[] arr;
    arr = nullptr;
    size = 0;
    capacity = 0;
  }
}

String::~String() {
  size = 0;
  capacity = 0;
  delete[] arr;
}

String& String::operator=(const String& string) {
  if (*this == string) return *this;
  String copy(string);
  this->Swap(copy);
  return *this;
}

String String::operator+(const String& string) const {
  String res(*this);
  res += string;
  return res;
}

String& String::operator+=(const String& string) {
  if (this->capacity <= this->size + string.size)
    this->Reserve((this->size + string.size) * 2);
  for (size_t i = 0; i <= string.size; ++i) arr[i + size] = string.arr[i];
  size += string.size;
  return *this;
}

String& String::operator*=(int x) {
  if (this->capacity <= this->size * x)
    this->MemoryRedefinitions((this->size * x) * 2);
  for (int j = 1; j < x; ++j)
    for (size_t i = 0; i < size; ++i) arr[i + size * j] = arr[i];
  size *= x;
  arr[size] = '\0';
  return *this;
}

String String::operator*(int i) const {
  String res(*this);
  res *= i;
  return res;
}
bool String::operator>(const String& string) const {
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] > string.arr[i]) return true;
    if (arr[i] < string.arr[i]) return false;
  }
  return false;
}
bool String::operator<(const String& string) const {
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] < string.arr[i]) return true;
    if (arr[i] > string.arr[i]) return false;
  }
  if (size < string.size) return true;
  return false;
}
bool String::operator>=(const String& string) const {
  return !(*this < string);
}
bool String::operator<=(const String& string) const {
  return !(*this > string);
}
bool String::operator==(const String& string) const {
  if (size != string.size) return false;
  for (size_t i = 0; i < size; ++i)
    if (arr[i] != string.arr[i]) return false;
  return true;
}
bool String::operator!=(const String& string) const {
  return !(*this == string);
}

char& String::operator[](int i) { return arr[i]; }

const char& String::operator[](int i) const { return arr[i]; }

void String::String::Clear() { size = 0; }

void String::String::PushBack(char character) {
  if (size + 1 >= capacity) MemoryRedefinitions((size + 1) * 2);
  ++size;
  arr[size - 1] = character;
  arr[size] = '\0';
}
void String::String::PopBack() {
  if (size > 0) {
    --size;
    arr[size] = '\0';
  }
}
void String::Resize(size_t new_size) {
  if (new_size >= capacity) MemoryRedefinitions(new_size + 1);
  size = new_size;
}
void String::Resize(size_t new_size, char character) {
  size_t i = size;
  Resize(new_size);
  for (; i < new_size; ++i) arr[i] = character;
  arr[new_size] = '\0';
}
void String::Reserve(size_t new_cap) {
  if (new_cap > capacity) MemoryRedefinitions(new_cap);
}
void String::ShrinkToFit() {
  if (capacity > size) {
    MemoryRedefinitions(size + 1);
    --capacity;
  }
}
void String::Swap(String& other) {
  char* temp_arr = other.arr;
  size_t temp_size = other.size;
  size_t temp_capacity = other.capacity;
  other.arr = this->arr;
  other.size = this->size;
  other.capacity = this->capacity;
  this->arr = temp_arr;
  this->size = temp_size;
  this->capacity = temp_capacity;
}
char& String::Front() { return arr[0]; }
char& String::Back() { return arr[size - 1]; }
const char& String::Front() const { return arr[0]; }
const char& String::Back() const { return arr[size - 1]; }

size_t String::Size() const { return this->size; }

size_t String::Capacity() const { return capacity; }

bool String::Empty() const { return this->size == 0; }
const char* String::Data() const { return arr; }

String String::Join(const std::vector<String>& strings) {
  if (strings.empty()) return "";
  size_t sum_size = 0;
  for (size_t i = 1; i < strings.size(); ++i) {
    sum_size += strings[i].Size();
  }
  String res(strings[0]);
  res.Reserve(sum_size + (strings.size() - 1) * size);
  for (size_t i = 1; i < strings.size(); ++i) {
    res += *this + strings[i];
  }
  return res;
}

size_t String::Find(const String& delim = " ", size_t begin = 0) {
  for (size_t i = begin; i < size - delim.size + 1; ++i) {
    size_t j = 0;
    for (j = 0; j < delim.size; j++) {
      if (arr[i + j] != delim[j]) {
        break;
      }
    }
    if (j == delim.size) {
      return i;
    }
  }
  return size;
}

String String::Substr(size_t start, size_t end) {
  String res(end - start);
  for (size_t i = start; i < end; ++i) res[i - start] = arr[i];
  return res;
}

std::vector<String> String::Split(const String& delim) {
  if (size < delim.size || size == 0) return {""};
  if (size == delim.size) {
    return {"", ""};
  }
  std::vector<String> result;
  size_t start = 0;
  size_t end = this->Find(delim);
  while (end != size) {
    result.push_back(this->Substr(start, end));
    start = end + delim.size;
    end = this->Find(delim, start);
  }
  result.push_back(this->Substr(start, end));
  return result;
}