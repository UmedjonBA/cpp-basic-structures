#pragma once

#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix();
  Matrix(const std::vector<std::vector<T>>& tmp);
  Matrix(const T& elem);
  Matrix(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator=(const Matrix<N, M, T>& mtrx);
  ~Matrix();
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator*=(const T& elem);

  Matrix<N, M, T> operator+(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T> operator-(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T> operator*(const T& elem) const;
  template <size_t K>
  Matrix<N, K, T> operator*(const Matrix<M, K, T>& mtrx) const;
  T& operator()(size_t row, size_t col);
  const T& operator()(size_t row, size_t col) const;

  bool operator==(const Matrix<N, M, T>& mtrx) const;
  bool operator!=(const Matrix<N, M, T>& mtrx) const;
  Matrix<M, N, T> Transposed() const;
  T Trace() const;

 private:
  std::vector<std::vector<T>> matrix_elements_;
};

template <size_t N, typename T>
T TraceF(const Matrix<N, N, T>& matrix);

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() {
  matrix_elements_.resize(N, std::vector<T>(M));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const T& elem) {
  matrix_elements_.resize(N, std::vector<T>(M, elem));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const std::vector<std::vector<T>>& tmp) {
  matrix_elements_ = tmp;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const Matrix<N, M, T>& mtrx) {
  matrix_elements_ = mtrx.matrix_elements_;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator=(const Matrix<N, M, T>& mtrx) =
    default;

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::~Matrix() = default;
template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& mtrx) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matrix_elements_[i][j] += mtrx.matrix_elements_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& mtrx) {
  return *this += mtrx * -1;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const T& elem) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matrix_elements_[i][j] *= elem;
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
template <size_t K>
Matrix<N, K, T> Matrix<N, M, T>::operator*(const Matrix<M, K, T>& mtrx) const {
  Matrix<N, K, T> res_of_mult;
  Matrix<K, M, T> tr_mtrx = mtrx.Transposed();
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < K; ++j) {
      for (size_t k = 0; k < M; ++k) {
        res_of_mult(i, j) += matrix_elements_[i][k] * tr_mtrx(j, k);
      }
    }
  }
  return res_of_mult;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix<N, M, T>& mtrx) {
  return *this += mtrx;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& mtrx) {
  return Matrix<N, M, T>(*this) -= mtrx;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(const T& elem) const {
  return Matrix<N, M, T>(*this) *= elem;
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t row, size_t col) {
  return matrix_elements_[row][col];
}

template <size_t N, size_t M, typename T>
const T& Matrix<N, M, T>::operator()(size_t row, size_t col) const {
  return matrix_elements_[row][col];
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() const {
  Matrix<M, N, T> mtrx;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      mtrx(j, i) = matrix_elements_[i][j];
    }
  }
  return mtrx;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& mtrx) const {
  return matrix_elements_ == mtrx.matrix_elements_;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator!=(const Matrix<N, M, T>& mtrx) const {
  return *this != mtrx;
}

template <size_t N, typename T>
T TraceF(const Matrix<N, N, T>& matrix) {
  T res_of_trace = T();
  for (size_t i = 0; i < N; ++i) {
    res_of_trace += matrix(i, i);
  }
  return res_of_trace;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() const {
  return TraceF(*this);
}
