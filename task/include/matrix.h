#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <iomanip>


/**
 * @brief Шаблонный класс матрицы.
 * 
 * @tparam N Количество строк в матрице
 * @tparam M Количество столбцов в матрице
 */
template<size_t N, size_t M>
class Matrix : public Vector<double, N*M> {
public:

    /**
     * @brief Конструктор по умолчанию.
     */
    Matrix() : Vector<double, N*M>() {}

    /**
     * @brief Конструктор из списка инициализации.
     *
     * @param list Список инициализации с элементами типа T
     */
    Matrix(std::initializer_list<double> list) 
        : Vector<double, N*M>(list) {}

    Matrix(Vector<double, N*M> father) 
        : Vector<double, N*M>(father) {}

    /**
     * @brief Транспонирование матрицы
     * 
     * @return Matrix<M, N> Транспонированная матрица размером M x N
     */
    Matrix<M, N> transpose() const {
        Matrix<M, N> result;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    /**
     * @brief Оператор доступа к элементам матрицы (неконстантная версия)
     * 
     * @param i Индекс строки (0 <= i < N)
     * @param j Индекс столбца (0 <= j < M)
     * @return Ссылка на элемент матрицы в позиции (i, j)
     */
    double& operator()(const size_t i, const size_t j) {
        return (*this)[i * M + j];
    }

    /**
     * @brief Оператор доступа к элементам матрицы (константная версия)
     * 
     * @param i Индекс строки (0 <= i < N)
     * @param j Индекс столбца (0 <= j < M)
     * @return Значение элемент матрицы в позиции (i, j)
     */
    double  operator()(const size_t i, const size_t j) const {
        return (*this)[i * M + j];
    }

};

/**
 * @brief Матричное умножение двух матриц.
 *
 * @tparam N Количество строк в первой матрице
 * @tparam M Количество столбцов в первой матрице и строк во второй матрице
 * @tparam L Количество столбцов во второй матрице
 * @param lhs Левая матрица (множимое) размером N x M
 * @param rhs Правая матрица (множитель) размером M x L
 * @return Matrix<N, L> Результирующая матрица размером N x L
 */
template<size_t N, size_t M, size_t L>
Matrix<N, L> operator*(const Matrix<N, M>& lhs, const Matrix<M, L>& rhs) {
    Matrix<N, L> result;
    Matrix<L, M> rhsT = rhs.transpose();
    
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < L; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < M; ++k) {
                sum += lhs(i, k) * rhsT(j, k);
            }
            result(i, j) = sum;
        }
    }
    
    return result;
}

/**
 * @brief Умножение матрицы на вектор (матрица-вектор произведение).
 *
 * @tparam N Количество строк в матрице
 * @tparam M Количество столбцов в матрице (должно быть равно размеру вектора)
 * @param matrix Матрица размером N x M
 * @param vector Вектор размером M
 * @return Vector<double, N> Результирующий вектор размером N
 */
template<size_t N, size_t M>
Vector<double, N> operator*(const Matrix<N, M>& matrix, const Vector<double, M>& vector) {
    Vector<double, N> result;
    for (size_t i = 0; i < N; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < M; ++j) {
            sum += matrix(i, j) * vector[j];
        }
        result[i] = sum;
    }
    return result;
}

/**
 * @brief Оператор вывода матрицы в поток
 * 
 * Форматирует матрицу для красивого вывода в консоль или файл.
 * 
 * @tparam N Количество строк в матрице
 * @tparam M Количество столбцов в матрице
 * @param os Выходной поток
 * @param matrix Матрица для вывода
 * @return Выходной поток
 */
template<size_t N, size_t M>
std::ostream& operator<<(std::ostream& os, const Matrix<N, M>& matrix) {
    std::ios_base::fmtflags old_flags = os.flags();
    std::streamsize old_precision = os.precision();
    os << std::fixed << std::setprecision(4);
    
    os << "Matrix<" << N << ", " << M << ">:\n";
    os << "[";
    
    for (size_t i = 0; i < N; ++i) {
        if (i > 0) {
            os << " ";
        }
        os << "[";
        for (size_t j = 0; j < M; ++j) {
            os << std::setw(8) << matrix(i, j);
            if (j < M - 1) {
                os << ", ";
            }
        }
        os << "]";
        if (i < N - 1) {
            os << ",\n";
        }
    }
    os << "]";
    
    os.flags(old_flags);
    os.precision(old_precision);
    
    return os;
}

#endif  // MATRIX_H 