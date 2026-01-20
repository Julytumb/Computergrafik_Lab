#include <cassert>
#include "matrix.h"

//Leerer Konstuktor
template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector(){

}
template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector( std::initializer_list<FLOAT_TYPE> values ) {
  auto iterator = values.begin();
  for (size_t i = 0u; i < N; i++) {
    if ( iterator != values.end()) {
      vector[i] = *iterator++;
    } else {
      vector[i] = (i > 0 ? vector[i - 1] : 0.0);
    }
  }
}

template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, N>::Vector(FLOAT_TYPE angle ) {
  *this = { static_cast<FLOAT_TYPE>( cos(angle) ), static_cast<FLOAT_TYPE>(sin(angle)) };
}

template <class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator+=(const Vector<FLOAT_TYPE, N> addend) {
  for (size_t i = 0u; i < N; i++) {
    vector[i] += addend.vector[i];
  }
  return *this;
}

template <class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator-=(const Vector<FLOAT_TYPE, N> minuend) {
  for (size_t i = 0u; i < N; i++) {
    vector[i] -= minuend.vector[i];
  }
  return *this;
}

template <class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator*=(const FLOAT_TYPE factor) {
  for (size_t i = 0u; i < N; i++) {
    vector[i] *= factor;
  }
  return *this;
}

template <class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE, N> & Vector<FLOAT_TYPE, N>::operator/=(const FLOAT_TYPE factor) {
  for (size_t i = 0u; i < N; i++) {
    vector[i] /= factor;
  }
  return *this;
}


template <class FLOAT_TYPE, size_t N>    
Vector<FLOAT_TYPE, N> operator*(FLOAT_TYPE scalar, Vector<FLOAT_TYPE, N> value) {
  Vector<FLOAT_TYPE, N> scalar_product = value;

  scalar_product *= scalar;

  return scalar_product;
}

template <class FLOAT_TYPE, size_t N>    
Vector<FLOAT_TYPE, N> operator+(const Vector<FLOAT_TYPE, N> value, const Vector<FLOAT_TYPE, N> addend) {
  Vector<FLOAT_TYPE, N> sum = value;
  sum += addend;
  return sum;
}

template <class FLOAT_TYPE, size_t N>    
Vector<FLOAT_TYPE, N> operator-(const Vector<FLOAT_TYPE, N> value, const Vector<FLOAT_TYPE, N> minuend) {
  Vector<FLOAT_TYPE, N> difference = value;
  difference -= minuend;
  return difference;
}

template <class FLOAT_TYPE, size_t N>  
FLOAT_TYPE & Vector<FLOAT_TYPE, N>::operator[](std::size_t i) {
  return vector[i];
}

template <class FLOAT_TYPE, size_t N>  
FLOAT_TYPE Vector<FLOAT_TYPE, N>::operator[](std::size_t i) const {
  return vector[i];
}


template <class FLOAT_TYPE, size_t N>
Vector<FLOAT_TYPE, 3u> Vector<FLOAT_TYPE, N>::cross_product(const Vector<FLOAT_TYPE, 3u> v) const {
  assert(N >= 3u);
  return {this->vector[1] * v.vector[2] - this->vector[2] * v.vector[1],
          this->vector[0] * v.vector[2] - this->vector[2] * v.vector[0],
          this->vector[0] * v.vector[1] - this->vector[1] * v.vector[0] };
}
/* NEU!! */
template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::square_of_length() const {
  FLOAT_TYPE sum = 0.0;
  for (size_t i = 0u; i < N; i++) {
    sum += this->vector[i] * this->vector[i];
  }
  return sum;
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::length() const {
  // Ruft die eben implementierte Funktion auf und zieht die Wurzel
  return std::sqrt(this->square_of_length());
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE operator*(Vector<FLOAT_TYPE, N> vector1, const Vector<FLOAT_TYPE, N> vector2) {
  FLOAT_TYPE sum = 0.0;
  for (size_t i = 0u; i < N; i++) {
    // Greift auf das "vector"-Array beider Vektoren zu
    sum += vector1.vector[i] * vector2.vector[i];
  }
  return sum;
}
/* NEU!! */


template <class FLOAT_TYPE, size_t N>  
void Vector<FLOAT_TYPE, N>::normalize() {
  *this /= length(); //  +/- INFINITY if length is (near to) zero
}

template <class FLOAT_TYPE, size_t N>  
Vector<FLOAT_TYPE, N> Vector<FLOAT_TYPE, N>::get_reflective(Vector<FLOAT_TYPE, N> normal) const {
  assert(0.99999 < normal.square_of_length() && normal.square_of_length()  < 1.000001); 
  return *this - static_cast<FLOAT_TYPE>(2.0) * (*this * normal ) * normal;
}

template <class FLOAT_TYPE, size_t N>
FLOAT_TYPE Vector<FLOAT_TYPE, N>::angle(size_t axis_1, size_t axis_2) const {
  Vector<FLOAT_TYPE, N> normalized = (1.0f / length()) * *this;
  return atan2( normalized[axis_2], normalized[axis_1] );
}

//Matrix Shit:
// Konstruktor: Erwartet eine Liste von Spalten-Vektoren
// template <class FLOAT, size_t N>
// SquareMatrix<FLOAT, N>::SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values) {
//     auto it = values.begin();
//     for (size_t i = 0u; i < N; i++) {
//         if (it != values.end()) {
//             matrix[i] = *it++; // Setzt den kompletten Vektor in die Spalte
//         } else {
//             // Falls die Liste zu kurz ist, mit Standard-Vektor füllen
//             matrix[i] = Vector<FLOAT, N>(); 
//         }
//     }
// }

// Zugriff auf die i-te Spalte (Referenz)
// template <class FLOAT, size_t N>
// Vector<FLOAT, N> & SquareMatrix<FLOAT, N>::operator[](std::size_t i) {
//     return matrix[i];
// }

// // Zugriff auf die i-te Spalte (Kopie/const)
// template <class FLOAT, size_t N>
// Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator[](std::size_t i) const {
//     return matrix[i];
// }

// // Zugriff auf ein einzelnes Element (Zeile, Spalte) - Spaltenweise Speicherung beachten!
// template <class FLOAT, size_t N>
// FLOAT SquareMatrix<FLOAT, N>::at(size_t row, size_t column) const {
//     // Da wir Spaltenvektoren speichern: erst Spalte wählen, dann Zeilen-Index
//     return matrix[column][row]; 
// }

// template <class FLOAT, size_t N>
// FLOAT & SquareMatrix<FLOAT, N>::at(size_t row, size_t column) {
//     return matrix[column][row];
// }

// // Matrix-Vektor-Multiplikation
// template <class FLOAT, size_t N>
// Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator*(const Vector<FLOAT, N> vector) {
//     Vector<FLOAT, N> result = Vector<FLOAT, N>();
//     // Da der Default-Konstruktor laut Aufgabe nichts macht, 
//     // müssen wir result explizit auf 0 setzen.
//     for (size_t i = 0; i < N; ++i) result[i] = 0;

//     for (size_t c = 0u; c < N; c++) {
//         // Ergebnis = Summe aus (Vektor-Komponente[c] * Spalten-Vektor[c])
//         result += vector[c] * matrix[c];
//     }
//     return result;
// }

// // Multiplikation zweier Matrizen (als freie Template-Funktion)
// template <class F, size_t K>
// SquareMatrix<F, K> operator*(const SquareMatrix<F, K> factor1, const SquareMatrix<F, K> factor2) {
//     SquareMatrix<F, K> result({}); 
//     for (size_t c = 0u; c < K; c++) {
//         // Die neue Spalte c ist factor1 multipliziert mit der Spalte c von factor2
//         result[c] = factor1 * factor2[c]; 
//     }
//     return result;
// }