#include "quaternion.hpp"

// Axis Angle Quaternion: Returns a unit quaternion that rotates by angle around axis. axis: any nonzero vector, normalized here. angle: in radians.
template<typename T> Quaternion<T> Quaternion<T>::axisAngleQuaternion(Vector3<T> axis, T angle) {
    axis = axis.vectorNormalized();
    T half = static_cast<T>(0.5) * angle;
    T s = std::sin(half);

    return Quaternion<T>{std::cos(half), axis.x * s, axis.y * s, axis.z * s};
}

// Quaternion Normalized: Returns this quaternion at unit length, or the identity if its magnitude is below 1e-8.
template<typename T> Quaternion<T> Quaternion<T>::quaternionNormalized() const {
    T magnitude = std::sqrt(w * w + i * i + j * j + k * k);
    T eps = static_cast<T>(1e-8);

    if (magnitude < eps) return Quaternion<T>{static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};

    return Quaternion<T>{w / magnitude, i / magnitude, j / magnitude, k / magnitude};
}

// Quaternion Inverse: Returns the inverse, or the identity if the squared magnitude is below 1e-8. For a unit quaternion, this is the reverse rotation.
template<typename T> Quaternion<T> Quaternion<T>::quaternionInverse() const {
    T magnitude_squared = w * w + i * i + j * j + k * k;
    T eps = static_cast<T>(1e-8);

    if (magnitude_squared < eps) return Quaternion<T>{static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)};

    return Quaternion<T>{w / magnitude_squared, -i / magnitude_squared, -j / magnitude_squared, -k / magnitude_squared};
}

// Operator *: Returns the Hamilton product. (this * other) rotates by other first, then by this. other: any quaternion.
template<typename T> Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& other) const {
    return Quaternion<T>{
        w * other.w - i * other.i - j * other.j - k * other.k,
        w * other.i + i * other.w + j * other.k - k * other.j,
        w * other.j - i * other.k + j * other.w + k * other.i,
        w * other.k + i * other.j - j * other.i + k * other.w
    };
}

// Quaternion Rotate Vector: Returns the vector rotated by this quaternion, normalized first. vector: any vector.
template<typename T> Vector3<T> Quaternion<T>::quaternionRotateVector(Vector3<T> vector) const {
    Quaternion<T> rotation = quaternionNormalized();
    Quaternion<T> quaternion_vector = {static_cast<T>(0), vector.x, vector.y, vector.z};
    Quaternion<T> vector_quaternion = rotation * quaternion_vector * rotation.quaternionInverse();
    
    return Vector3<T>{vector_quaternion.i, vector_quaternion.j, vector_quaternion.k};
}

template struct Quaternion<float>;
template struct Quaternion<double>;
