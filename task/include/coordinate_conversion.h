#ifndef COORDINATE_CONVERSION_H
#define COORDINATE_CONVERSION_H

#include <cmath>

#include "cartesian.h"
#include "keplerian.h"
#include "matrix.h"
#include "vector3d.h"
#include "runge_kutta.h"
#include "keplersolver.h"

// Forward declarations
Matrix<3, 3> rotation_x(const double angle);
Matrix<3, 3> rotation_y(const double angle);
Matrix<3, 3> rotation_z(const double angle);
double true_to_ecc(double nu, double e);
double ecc_to_true(double E, double e);
double ecc_to_mean(double E, double e);
double mean_to_ecc(double M, double e);



// Выполняет преобразование положения и скорости в кеплеровы элементы
// @param cart положение и скорость
// @param mu - гравитационный параметр
Keplerian to_keplerian(const Cartesian& cart, const double mu) {
    const auto& [r, v] = cart;
    const Vector3d L = cross(r, v);
    const double norm_L = L.length();
    const double cos_L = L.z() / norm_L;
    const double sin_L = std::sqrt(L.x() * L.x() + L.y() * L.y()) / norm_L;
    const double i = std::atan2(sin_L, cos_L);

    const Vector3d z = Vector3d{0, 0, 1};

    const Vector3d N_exp = cross(z, L);
    const double N_norm = N_exp.length();
    const Vector3d N = N_norm == 0 ? Vector3d{1, 0, 0} : N_exp / N_norm;

    const double raan = std::atan2(N.y(), N.x());

    const double r_norm = r.length();
    const Vector3d e_vec = ((v.dot(v) - mu / r_norm) * r - r.dot(v) * v) / mu;

    const double e_norm = e_vec.length();
    const Vector3d e_1 = e_norm == 0 ? N : Vector3d{e_vec / e_norm};

    const Vector3d d_1 = N;
    const Vector3d d_2 = cross(L, N) / norm_L;

    const double cos_w = d_1.dot(e_1);
    const double sin_w = d_2.dot(e_1);
    const double w = std::atan2(sin_w, cos_w);

    const Vector3d e_2 = cross(L, e_1) / norm_L;
    const double cos_nu = e_1.dot(r);
    const double sin_nu = e_2.dot(r);
    const double nu = std::atan2(sin_nu, cos_nu);

    const double a = -mu / (v.dot(v) / 2 - mu / r_norm) / 2;

    return Keplerian{.a = a, .e = e_norm, .w = w, .i = i, .raan = raan, .nu = nu};
}

// Выполняет преобразование кеплеровых элементов в положение и скорость
// @param kepl кеплеровы элементы
// @param mu гравитационный параметр
Cartesian to_cartesian(const Keplerian& k, double mu) {
    const double p = k.a * (1 - k.e * k.e);  // фокальный параметр

    const double sin = std::sin(k.nu);
    const double cos = std::cos(k.nu);

    const double r = p / (1 + k.e * cos);

    const Vector3d r_vec = Vector3d{cos, sin, 0} * r;

    const double p_mu = std::sqrt(p / mu);

    const double r_dot = p_mu * k.e * sin;
    const double rv_dot = p_mu * (1 + k.e * cos);

    const Vector3d v_vec = Vector3d{r_dot * cos - rv_dot * sin, r_dot * sin + rv_dot * cos, 0};

    const Matrix<3, 3> Q = rotation_z(-k.raan) * rotation_x(-k.i) * rotation_z(-k.w);

    return Cartesian{.r = Q * r_vec, .v = Q * v_vec};
}

// Выпонляет прогноз движения
// @param cart0 началньые положения и скорость
// @param промежуток времени прогноза
// @param mu гравитационный параметр
Cartesian propagate(const Cartesian& cart0, double dt, double mu) {
    const auto kep0 = to_keplerian(cart0, mu);
    const auto e    = kep0.e;
    const auto a    = kep0.a;
    const auto nu0  = kep0.nu;
    const auto E0   = true_to_ecc(nu0, e);  // переводит истинную аномалию в эксцентрическую
    const auto M0   = ecc_to_mean(E0, e);  // переводит эксцентрическую аномалию в средниюю
    const auto n    = std::sqrt(mu / a / a / a);  // среднее движение
    const auto M    = M0 + n * dt;
    const auto E    = mean_to_ecc(M, e);  // решаем уравнение Кеплера
    const auto nu   = ecc_to_true(E, e);  // переводит эксцентрическую аномалию в истинную

    const auto kep1 = Keplerian{kep0.a, kep0.e, kep0.w, kep0.i, kep0.raan, nu};

    return to_cartesian(kep1, mu);
}


Matrix<3, 3> rotation_x(const double angle) {
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    
    return Matrix<3, 3>{
        1,      0,     0,
        0,  cos_a, sin_a,
        0, -sin_a, cos_a
    };
}

Matrix<3, 3> rotation_y(const double angle) {
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    
    return Matrix<3, 3>{
        cos_a, 0, -sin_a,
            0, 1,      0,
        sin_a, 0,  cos_a
    };
}

Matrix<3, 3> rotation_z(const double angle) {
    const double cos_a = std::cos(angle);
    const double sin_a = std::sin(angle);
    
    return Matrix<3, 3>{
        cos_a,  sin_a, 0,
        -sin_a, cos_a, 0,
             0,     0, 1
    };
}


double true_to_ecc(double nu, double e) {
    const double cos_nu = std::cos(nu);
    const double sin_nu = std::sin(nu);
    const double sin_E = sin_nu * std::sqrt(1 - e * e) / (1 + e * cos_nu);
    const double cos_E = (e + cos_nu) / (1 + e * cos_nu);
    const double E = std::atan2(sin_E, cos_E);
    return E;
}

// Преобразование эксцентрической аномалии в истинную
double ecc_to_true(double E, double e) {
    const double sin_E = std::sin(E);
    const double cos_E = std::cos(E);
    const double sin_nu = sin_E * std::sqrt(1 - e * e) / (1 - e * cos_E);
    const double cos_nu = (cos_E - e) / (1 - e * cos_E);
    const double nu = std::atan2(sin_nu, cos_nu);
    return nu;
}

// Преобразование эксцентрической аномалии в среднюю
double ecc_to_mean(double E, double e) {
    const double M = E - e * std::sin(E);
    return M;
}

// Решение уравнения Кеплера: M = E - e*sin(E)
// Используем метод Ньютона-Рафсона
double mean_to_ecc(double M, double e) {
  return solveKeplerEquationNewton(M, e);
}



#endif  // COORDINATE_CONVERSION_H