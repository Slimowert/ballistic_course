#include <iostream>
#include <fstream>
#include <iomanip>
#include "vector3d.h"
#include "vector.h"
#include "runge_kutta.h"
#include "cartesian.h"
#include "coordinate_conversion.h"

const double DEG_TO_RAD = PI / 180.0;
const double RAD_TO_DEG = 180.0 / PI;

Vector<double, 6> rhs(double, const Vector<double, 6>& y) {
    const Vector3d pos{ y[0], y[1], y[2] };
    const Vector3d vel{ y[3], y[4], y[5] };
    // y' = (vx, vy, vz, ax, ay, az);

    const double mu = 3.986004415e14;
    const double R = 6381e3;
    const double J2 = 1.08e-3;
    const double r1 = pos.length();
    const double r2 = r1 * r1;
    const double r3 = r2 * r1;
    const Vector3d acc_main = -mu / r3 * pos;

    const double epsilon = 3 * mu * R * R * J2 / 2;
    const double r5 = r3 * r2;
    const double r7 = r5 * r2;
    const double rx = pos.x();
    const double ry = pos.y();
    const double rz = pos.z();

    const double corrAccX =
        -epsilon * rx * (rx * rx + ry * ry - 4 * rz * rz) / r7;
    const double corrAccY =
        -epsilon * ry * (rx * rx + ry * ry - 4 * rz * rz) / r7;
    const double corrAccZ =
        epsilon * rz * (-3 * rx * rx - 3 * ry * ry + 2 * rz * rz) / r7;

    const Vector3d acc_cor{ corrAccX, corrAccY, corrAccZ };

    const Vector3d acc = acc_main + acc_cor;

    const Vector<double, 6> res{ vel.x(), vel.y(), vel.z(), acc.x(), acc.y(), acc.z() };
    return res;
}

int main() {

    const double t0 = 0;
    const double tfin = 86400;
    const double h = 30;
    const double mu = 3.986004415e14;

    Keplerian orb1 = {
        6800e3,    // большая полуось в метрах
        0.05,      // эксцентриситет
        0.0,       // аргумент перицентра в градусах
        55.0 * DEG_TO_RAD,      // наклонение в градусах
        0.0,       // долгота восходящего узла в градусах
        0.0        // истинная аномалия в градусах
    };
    
    Keplerian orb2 = {
        7000e3,    // большая полуось в метрах
        0.05,      // эксцентриситет
        0.0,       // аргумент перицентра в градусах
        63.4 * DEG_TO_RAD,      // наклонение в градусах
        0.0,       // долгота восходящего узла в градусах
        0.0        // истинная аномалия в градусах
    };

    const Cartesian cart1 = to_cartesian(orb1, mu);
    
    const Cartesian cart2 = to_cartesian(orb2, mu);
    
    
    {
        //const Vector3d r{ 10000e3, 0, 0};
        //const Vector3d v{0, 0, 5000};
        //const Vector<double, 6> y0_orb1{r.x(), r.y(), r.z(), v.x(), v.y(), v.z()};
        const Vector<double, 6> y0_orb1{ cart1.r.x(), cart1.r.y(), cart1.r.z(), cart1.v.x(), cart1.v.y(), cart1.v.z() };
        std::ofstream f1("res_orb1.csv");
        f1 << std::setprecision(15);
        f1 << "t;x;y;z;vx;vy;vz;a;e;w;i;raan;nu" << std::endl;
        
        for (double t1 = 0; t1 < tfin; t1 += 30) {
            const auto y1 = rk4(rhs, t0, y0_orb1, t1, h);
            const Vector3d r_current{ y1[0], y1[1], y1[2] };
            const Vector3d v_current{ y1[3], y1[4], y1[5] };
            const Cartesian cart{ r_current, v_current };
            const auto kep = to_keplerian(cart, mu);
            
            f1 << t1 << ';'
               << r_current.x() << ';' << r_current.y() << ';' << r_current.z() << ';'
               << v_current.x() << ';' << v_current.y() << ';' << v_current.z() << ';'
               << kep.a << ';' << kep.e << ';' << kep.w << ';' << kep.i << ';' << kep.raan << ';' << kep.nu
               << std::endl;
        }
    }
    
    {
        const Vector<double, 6> y0_orb2{ cart2.r.x(), cart2.r.y(), cart2.r.z(), cart2.v.x(), cart2.v.y(), cart2.v.z() };
        std::ofstream f2("res_orb2.csv");
        f2 << std::setprecision(15);
        f2 << "t;x;y;z;vx;vy;vz;a;e;w;i;raan;nu" << std::endl;
        
        for (double t1 = 0; t1 < tfin; t1 += 30) {
            const auto y1 = rk4(rhs, t0, y0_orb2, t1, h);
            const Vector3d r_current{ y1[0], y1[1], y1[2] };
            const Vector3d v_current{ y1[3], y1[4], y1[5] };
            const Cartesian cart{ r_current, v_current };
            const auto kep = to_keplerian(cart, mu);
            
            f2 << t1 << ';'
               << r_current.x() << ';' << r_current.y() << ';' << r_current.z() << ';'
               << v_current.x() << ';' << v_current.y() << ';' << v_current.z() << ';'
               << kep.a << ';' << kep.e << ';' << kep.w << ';' << kep.i << ';' << kep.raan << ';' << kep.nu
               << std::endl;
        }
    }
    

}