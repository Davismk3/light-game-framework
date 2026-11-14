#pragma once

#include "vector.hpp"

#include <cmath>

// Quaternion: A rotation in 3D, stored as w + i + j + k.
template<typename T> struct Quaternion {

    // Components
    T w;  // real part, cos(angle / 2) for a unit rotation
    T i;  // x of the rotation axis, times sin(angle / 2)
    T j;  // y of the rotation axis, times sin(angle / 2)
    T k;  // z of the rotation axis, times sin(angle / 2)

    static Quaternion<T> axisAngleQuaternion(Vector3<T> axis, T angle);

    Quaternion<T> quaternionNormalized() const;
    Quaternion<T> quaternionInverse() const;

    Quaternion<T> operator*(const Quaternion& other) const;
    Vector3<T> quaternionRotateVector(Vector3<T> vector) const;
};

using QuaternionF = Quaternion<float>;
using QuaternionD = Quaternion<double>;
