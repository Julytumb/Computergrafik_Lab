#include <cassert>

// Konstruktor
template <class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values) {
  auto it = values.begin();
  for (size_t i = 0u; i < N; i++) {
    if (it != values.end()) {
      matrix[i] = *it++;
    } else {
      // Manuelle Initialisierung, falls Liste zu kurz
      for (size_t j = 0; j < N; ++j) matrix[i][j] = 0.0f;
    }
  }
}

// Spaltenzugriff
template <class FLOAT, size_t N>
Vector<FLOAT, N> & SquareMatrix<FLOAT, N>::operator[](std::size_t i) {
  return matrix[i];
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator[](std::size_t i) const {
  return matrix[i];
}

// Elementzugriff (Column-Order: Spalte zuerst)
template <class FLOAT, size_t N>
FLOAT SquareMatrix<FLOAT, N>::at(size_t row, size_t column) const {
  return matrix[column][row];
}

template <class FLOAT, size_t N>
FLOAT & SquareMatrix<FLOAT, N>::at(size_t row, size_t column) {
  return matrix[column][row];
}

// Matrix-Vektor-Produkt (Nicht-konstant laut matrix.h)
template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator*(const Vector<FLOAT, N> vector) {
  Vector<FLOAT, N> result;
  // Da Vector() laut Aufgabe nichts tut, müssen wir nullen
  for (size_t i = 0; i < N; ++i) result[i] = 0.0f;

  for (size_t c = 0u; c < N; c++) {
    result += vector[c] * matrix[c];
  }
  return result;
}

// Matrix-Matrix-Produkt (Friend Funktion)
template <class F, size_t K>
SquareMatrix<F, K> operator*(const SquareMatrix<F, K> factor1, const SquareMatrix<F, K> factor2) {
  SquareMatrix<F, K> result({});
  
  // WORKAROUND: Da SquareMatrix::operator*(Vector) in matrix.h nicht const ist,
  // müssen wir factor1 kopieren, um die Methode aufrufen zu dürfen.
  SquareMatrix<F, K> factor1_non_const = factor1;

  for (size_t c = 0u; c < K; c++) {
    // Ruft jetzt den Matrix-Vektor-Operator auf der Kopie auf
    result[c] = factor1_non_const * factor2[c];
  }
  return result;
}