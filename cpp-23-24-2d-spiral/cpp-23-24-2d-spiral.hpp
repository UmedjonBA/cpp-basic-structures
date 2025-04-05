int** MakeSpiral(int n) {
  int** arr = new int*[n];
  for (int l = 0; l < n; l++) arr[l] = new int[n];
  int iterator_by_line = 0;
  int iterator_by_column = 0;
  int cell_values = 1;
  int step = 1;
  int end_iterator_by_line = 0;
  int end_iterator_by_column = n - 1;
  while (cell_values <= n * n) {
    arr[iterator_by_column][iterator_by_line] = cell_values;
    cell_values++;
    if (step == 1) {
      if (iterator_by_line < end_iterator_by_column)
        iterator_by_line++;
      else
        iterator_by_column++;
    } else {
      if (iterator_by_line > end_iterator_by_line)
        iterator_by_line--;
      else
        iterator_by_column--;
    }
    if (iterator_by_line == end_iterator_by_line &&
        iterator_by_column - 1 == end_iterator_by_line) {
      step = 1;
      end_iterator_by_line++;
    } else if (iterator_by_line == end_iterator_by_column &&
               iterator_by_column == end_iterator_by_column) {
      step = -1;
      end_iterator_by_column--;
    }
  }
  return arr;
}
