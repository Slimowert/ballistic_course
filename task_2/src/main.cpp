#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main() {
    // 1. Создание векторов
    Vector3f v1(1.0f, 2.0f, 3.0f);
    VectorXd v2(5);
    v2 << 1, 2, 3, 4, 5;

    cout << "Vector v1:\n" << v1 << endl;
    cout << "Vector v2:\n" << v2 << endl;

    // 2. Создание матриц
    Matrix3f m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;

    MatrixXd m2(2, 3);
    m2 << 1, 2, 3,
          4, 5, 6;

    cout << "Matrix m1:\n" << m1 << endl;
    cout << "Matrix m2:\n" << m2 << endl;

    // 3. Доступ к элементам
    cout << "Element (0,0) of m1: " << m1(0,0) << endl;
    cout << "First element of v1: " << v1[0] << endl;

    // 4. Размерности
    cout << "Rows of m2: " << m2.rows() << endl;
    cout << "Cols of m2: " << m2.cols() << endl;
    cout << "Size of v2: " << v2.size() << endl;

    return 0;
}