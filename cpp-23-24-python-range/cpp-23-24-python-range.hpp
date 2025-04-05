#include <utility>

std::pair<int, const int*> Range(int from, int to, int step = 1) {
  if (step == 0 || (to - from) / step <= 0) {
    int* empty_arr = new int[0];
    return std::make_pair(0, empty_arr);
  }
  int sign = (from < to) ? 1 : -1;
  int sign_step = (step > 0) ? 1 : -1;
  if ((from > 0 && to > 0) || (from < 0 && to < 0)) sign_step = 0;
  int size = (to - from + sign_step) / step;
  int* res = new int[size];
  int counter = 0;
  for (int i = from; sign * i < sign * to; i += step) {
    res[counter] = i;
    counter++;
  }
  return std::make_pair(size, res);
}