#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

/**
 * @brief Шаблонный класс для представления вектора фиксированного размера.
 *
 * @tparam T Тип элементов вектора (должен поддерживать арифметические операции)
 * @tparam N Размер вектора (должен быть > 0)
 */
template<typename T, size_t N>
class Vector {

public:

    /**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует все элементы значением по умолчанию типа T.
     */
    Vector() : data_() {};

    /**
     * @brief Конструктор из списка инициализации.
     *
     * @param list Список инициализации с элементами типа T
     *
     * Если список содержит меньше элементов чем N, остальные элементы
     * инициализируются значением по умолчанию типа T.
     * Если список содержит больше элементов чем N, лишние элементы игнорируются.
     */
    Vector(std::initializer_list<T> list) {
        size_t i = 0;
        for (const auto& value : list) {
            if (i < N) {
                data_[i++] = value;
            }
        }
        for (; i < N; ++i) {
            data_[i] = T();
        }
    }

    /**
     * @brief Оператор доступа к элементу по индексу.
     *
     * @param index Индекс элемента (0 <= index < N)
     * @return Ссылка на элемент
     */
    T& operator[](size_t index) {
        return data_[index];
    }

    /**
     * @brief Константный оператор доступа к элементу по индексу.
     *
     * @param index Индекс элемента (0 <= index < N)
     * @return Константная ссылка на элемент
     */
    T operator[](size_t index) const {
        return data_[index];
    }

    /**
     * @brief Безопасный доступ к элементу по индексу с проверкой границ.
     *
     * @param index Индекс элемента
     * @return Ссылка на элемент
     * @throw std::out_of_range Если index >= N
     */
    T& at(size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    /**
     * @brief Константный безопасный доступ к элементу по индексу с проверкой границ.
     *
     * @param index Индекс элемента
     * @return Константная ссылка на элемент
     * @throw std::out_of_range Если index >= N
     */
    T at(size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    /**
     * @brief Унарный плюс.
     *
     * @return Копия текущего вектора
     */
    Vector operator+() const {
        return *this;
    }

    /**
     * @brief Унарный минус.
     *
     * @return Вектор с противоположными по знаку элементами
     */
    Vector operator-() const {
        Vector result;
        for (size_t i = 0; i < N; ++i) {
            result.data_[i] = -data_[i];
        }
        return result;
    }

    /**
     * @brief Оператор сложения с присваиванием.
     *
     * @param other Вектор для сложения
     * @return Ссылка на измененный текущий вектор
     */
    Vector& operator+=(const Vector& other) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    /**
     * @brief Оператор вычитания с присваиванием.
     *
     * @param other Вектор для вычитания
     * @return Ссылка на измененный текущий вектор
     */
    Vector& operator-=(const Vector& other) {
        *this += -other;
        return *this;
    }

    /**
     * @brief Оператор вычитания векторов.
     *
     * @param other Вектор для вычитания
     * @return Результат вычитания
     */
    Vector operator-(const Vector& other) const {
        return Vector(*this) -= other;
    }

    /**
     * @brief Оператор сложения векторов.
     *
     * @param other Вектор для сложения
     * @return Результат сложения
     */
    Vector operator+(const Vector& other) const {
        return Vector(*this) += other;
    }

    /**
     * @brief Оператор умножения на скаляр с присваиванием.
     *
     * @param scalar Скалярный множитель
     * @return Ссылка на измененный текущий вектор
     */
    Vector& operator*=(T scalar) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] *= scalar;
        }
        return *this;
    }

    /**
     * @brief Оператор деления на скаляр с присваиванием.
     *
     * @param scalar Скалярный делитель
     * @return Ссылка на измененный текущий вектор
     * @throw std::invalid_argument Если scalar == 0
     */
    Vector& operator/=(T scalar) {
        if (scalar == T()) {
            throw std::invalid_argument("Division by zero");
        }
        for (size_t i = 0; i < N; ++i) {
            data_[i] /= scalar;
        }
        return *this;
    }

    /**
     * @brief Оператор умножения на скаляр.
     *
     * @param scalar Скалярный множитель
     * @return Vector Результат умножения
     */
    Vector operator*(T scalar) const {
        return Vector(*this) *= scalar;
    }

    /**
     * @brief Оператор деления на скаляр.
     *
     * @param scalar Скалярный делитель
     * @return Результат деления
     * @throw std::invalid_argument Если scalar == 0
     */
    Vector operator/(T scalar) const {
        return Vector(*this) /= scalar;
    }

    /**
     * @brief Скалярное произведение векторов.
     *
     * @param other Второй вектор для произведения
     * @return Результат скалярного произведения
     */
    T dot(const Vector& other) const {
        T result = T();
        for (size_t i = 0; i < N; ++i) {
            result += data_[i] * other.data_[i];
        }
        return result;
    }

    /**
     * @brief Вычисление длины вектора.
     *
     * @return Длина вектора
     */
    T length() const {
        return std::sqrt(dot(*this));
    }

    /**
     * @brief Нормализация вектора (приведение к единичной длине).
     *
     * @return Нормализованный вектор
     * @throw std::runtime_error Если длина вектора равна нулю
     */
    Vector normalized() const {
        T len = length();
        if (len == T()) {
            throw std::runtime_error("Cannot normalize zero vector");
        }
        return *this / len;
    }

    /**
     * @brief Получение размера вектора.
     *
     * @return Размер вектора N
     */
    constexpr size_t size() { return N; }

    /**
     * @brief Оператор вывода вектора в поток.
     *
     * @param os Выходной поток
     * @param vec Вектор для вывода
     * @return Ссылка на выходной поток
     *
     * Формат вывода: [x_{0}, x_{1}, \ldots, x_{N-1}]
     */
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << "[";
        for (size_t i = 0; i < N; ++i) {
            os << vec.data_[i];
            if (i < N - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

private:
    std::array<T, N> data_;  ///< Внутренний массив для хранения данных вектора
};

/**
 * @brief Оператор умножения скаляра на вектор.
 *
 * @param scalar Скалярный множитель
 * @param vec Вектор
 * @return Результат умножения
 */
template<typename T, size_t N>
Vector<T, N> operator*(T scalar, const Vector<T, N>& vec) {
    return vec * scalar;
}

#endif  // VECTOR_H