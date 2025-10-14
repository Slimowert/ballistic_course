#ifndef CARTESIAN_H
#define CARTESIAN_H

#include "vector3d.h"

/**
 * @brief Параметры в декартовой системе координат
 * 
 */
struct Cartesian {
    Vector3d r;  ///< Положение [м]
    Vector3d v;  ///< Скорости [м/с]
};

#endif  // CARTESIAN_H
