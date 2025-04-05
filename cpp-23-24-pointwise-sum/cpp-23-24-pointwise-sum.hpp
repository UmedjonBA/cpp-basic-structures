int* PointwiseSum(const int* a, const int* b, int n) {
  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    arr[i] = a[i] + b[i];
  }
  return arr;
}