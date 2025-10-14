#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "vector.h"


/**
 * @brief Класс трехмерного вектора.
 *
 */
class Vector3d : public Vector<double, 3> {

public:

    /**
     * @brief Конструктор по умолчанию.
     */
    Vector3d() : Vector<double, 3>() {}

    /**
     * @brief Конструктор из списка инициализации.
     *
     * @param list Список инициализации с элементами типа T
     */
    Vector3d(std::initializer_list<double> list) 
        : Vector<double, 3>(list) {}

    Vector3d(Vector<double, 3> father) 
        : Vector<double, 3>(father) {}

    /**
     * @brief Получить x-компоненту вектора (константная версия)
     * 
     * @return Значение x-координаты
     */
    double x() const {
        return (*this)[0];
    }

    /**
     * @brief Получить x-компоненту вектора (неконстантная версия)
     * 
     * @return Ссылка на x-координату для модификации
     */
    double& x() {
        return (*this)[0];
    }

    /**
     * @brief Получить y-компоненту вектора (константная версия)
     * 
     * @return Значение y-координаты
     */
    double y() const {
        return (*this)[1];
    }

    /**
     * @brief Получить y-компоненту вектора (неконстантная версия)
     * 
     * @return Ссылка на y-координату для модификации
     */
    double& y() {
        return (*this)[1];
    }

    /**
     * @brief Получить z-компоненту вектора (константная версия)
     * 
     * @return Значение z-координаты
     */
    double z() const {
        return (*this)[2];
    }

    /**
     * @brief Получить z-компоненту вектора (неконстантная версия)
     * 
     * @return Ссылка на z-координату для модификации
     */
    double& z() {
        return (*this)[2];
    }

    /**
     * @brief Векторное произведение
     * 
     * @param other Вектор для вычисления векторного произведения
     * @return Вектор, являющийся результатом векторного произведения
     */
    Vector3d cross(const Vector3d& other) const {
        return Vector3d{
            y() * other.z() - z() * other.y(),
            z() * other.x() - x() * other.z(),
            x() * other.y() - y() * other.x()
        };
    }
};

/**
 * @brief Векторное произведение
 * @param lhs Вектор для вычисления векторного произведения
 * @param rhs Вектор для вычисления векторного произведения
 * @return Vector3d Вектор, являющийся результатом векторного произведения
 */
Vector3d cross(const Vector3d& lhs, const Vector3d& rhs) {
    return Vector3d{
        lhs.y() * rhs.z() - lhs.z() * rhs.y(),
        lhs.z() * rhs.x() - lhs.x() * rhs.z(),
        lhs.x() * rhs.y() - lhs.y() * rhs.x()
    };
}

#endif  // VECTOR3D_H