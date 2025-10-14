#ifndef RUNGE_KUTTA_H
#define RUNGE_KUTTA_H

#include <algorithm>


/**
 * @brief Реализация метода Рунге-Кутты 4-го порядка для решения ОДУ
 * 
 * @tparam F Функции правой части системы ОДУ
 * @tparam T Переменной времени
 * @tparam Y Переменной состояния
 * @tparam H Шага интегрирования
 * 
 * @param f Функция правой части системы ОДУ вида: y' = f(t, y)
 * @param t_0 Начальное время
 * @param y_0 Начальное значение функции в момент времени t_0
 * @param t_1 Конечное время, до которого производится интегрирование
 * @param h_0 Начальный шаг интегрирования
 * 
 * @return Y Значение функции в момент времени t_1
 */
template<typename F, typename T, typename Y, typename H>
Y rk4(
    const F& f,
    const T& t_0,
    const Y& y_0,
    const T& t_1,
    const H& h_0
) {
    auto t = t_0;
    auto y = y_0;
    while (t < t_1){
        const auto h = std::min(h_0, t_1 - t);
        const auto k_0 = f(t, y);
        const auto k_1 = f(t + h / 2, y + h / 2 * k_0);
        const auto k_2 = f(t + h / 2, y + h / 2 * k_1);
        const auto k_3 = f(t + h    , y + h     * k_2);

        y = y + h / 6 * (k_0 + 2 * k_1 + 2 * k_2 + k_3);
        t = std::min(t + h, t_1);
    }

    return y;
}

#endif  // RUNGE_KUTTA_H