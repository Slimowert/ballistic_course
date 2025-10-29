#include <cmath>
#include <limits>

constexpr double PI = 3.14159265358979323846;

// Решение уравнение Кеплера
// E - e * sin(E) = M, E - ?, M - число, e - число [0, 1).
// E - эксцентрическая аномалия.
// M - средняя аномалия.
// e - эксцентриситет орбиты.
// Уравнение Кеплера нельзя разрешить в виде алгебраической формулы.
//
// Численные методы для решения алгебраических уравнений:
// - Метод Ньютона
// - Метод половинного деления (дихотомия)
// - Метод простой итерации.
// 
// Метод простой итерации.
// f(x) = 0 <=> x = g(x).
// x_{i + 1} = g(x_{i}), x_0 - начальное приближение.
// ТЕОРЕМА: МПИ сходится, если | g(x)'| < 1.

/**
 * @brief Решает уравнение Кеплера методом простой итерации.
 *
 * Уравнение Кеплера: E - e·sin(E) = M
 * Преобразованное для итерации: E = e·sin(E) + M
 *
 * Теорема о сходимости: |g'(E)| = e·|cos(E)| ≤ e < 1, метод сходится.
 *
 * @param e - Эксцентриситет.
 * @param M - Средняя аномалия, [рад].
 * @param tol - допустимая точность решения.
 * @param max_iter Максимальное количество итераций.
 *
 * @return Эксцентрическая аномалия, [рад].
 */
double solveKeplerEquationMSI(
    const double e,
    const double M,
    const double tol = std::numeric_limits<double>::epsilon(),
    const size_t max_iter = 1000
    ) {

    if (e < 0.0 || e > 1.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double current_E = (e < 0.8) ? M : PI;
    size_t iteration = 0;

    while (iteration < max_iter) {
        double next_E = e * std::sin(current_E) + M;

        double delta = std::abs(next_E - current_E);
        if (delta <= tol) {
            return next_E;
        }

        current_E = next_E;
        iteration++;
    }
    return std::numeric_limits<double>::quiet_NaN();
}

/**
 * Решает уравнение Кеплера: E - e * sin(E) = M
 * методом Ньютона (методом касательных)
 *
 * @param e - Эксцентриситет.
 * @param M - Средняя аномалия, [рад].
 * @param tol - допустимая точность решения.
 * @param max_iter Максимальное количество итераций.
 *
 * @return Эксцентрическая аномалия, [рад].
 */
double solveKeplerEquationNewton(
    const double e,
    const double M,
    const double tol = std::numeric_limits<double>::epsilon(),
    const size_t max_iter = 100
    ) {
    if (e < 0.0 || e > 1.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double E = (e < 0.8) ? M : PI;
    size_t iteration = 0;
    double delta;

    do {
        double f = E - e * std::sin(E) - M;
        double f_prime = 1.0 - e * std::cos(E);
        if (std::abs(f_prime) < tol) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        delta = -f / f_prime;
        E += delta;
        ++iteration;
        if (iteration > max_iter) {
            return std::numeric_limits<double>::quiet_NaN();
        }
    } while (std::abs(delta) > tol);

    return E;
}