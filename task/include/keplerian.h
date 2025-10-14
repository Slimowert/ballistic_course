#ifndef KEPLERIAN_H
#define KEPLERIAN_H

/**
 * @brief Кеплеровы параметры (элементы)
 * 
 */
struct Keplerian {
    double a;     ///< Большая полуось
    double e;     ///< Эксцентриситет
    double w;     ///< Аргумент перицентра
    double i;     ///< Наклонение
    double raan;  ///< Долгота восходящего узла
    double nu;    ///< Истинная аномалия
};

#endif  // KEPLERIAN_H