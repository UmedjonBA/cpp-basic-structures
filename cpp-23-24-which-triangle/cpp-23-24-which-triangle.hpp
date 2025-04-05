#pragma once

#include <algorithm>

/*
 * Верните 0 если и только если треугольник невозможно построить
 * Верните 1 если и только если треугольник вырожденный
 * Верните 2 если и только если треугольник остроугольный
 * Верните 3 если и только если треугольник прямоугольный
 * Верните 4 если и только если треугольник тупоугольный
 */

int max(int a, int b, int c) {
  if (a >= b && a >= c) return a;
  if (b >= a && b >= c) return b;
  if (c >= a && c >= b) return c;
}
int min(int a, int b, int c) {
  if (a <= b && a <= c) return a;
  if (b <= a && b <= c) return b;
  if (c <= a && c <= b) return c;
}
int WhichTriangle(int a, int b, int c) {
  if (a <= 0 || b <= 0 || c <= 0 || a + b < c || a + c < b || b + c < a) {
    return 0;
  }
  if (a + b <= c || a + c <= b || b + c <= a) {
    return 1;
  }
  int max_side = max(a, b, c);
  int min_side = min(a, b, c);
  int mid_side = a + b + c - max_side - min_side;
  long long res =
      static_cast<long long>(max_side) * static_cast<long long>(max_side) -
      static_cast<long long>(min_side) * static_cast<long long>(min_side) -
      static_cast<long long>(mid_side) * static_cast<long long>(mid_side);
  if (res == 0) {
    return 3;
  } else if (res > 1) {
    return 4;
  } else if (res < 1) {
    return 2;
  }
  return -1;
}