#pragma once

#include <cmath>

// Vector4: Four components of type T.
template <typename T> struct Vector4 {

    // Components
    T x;  // first component
    T y;  // second component
    T z;  // third component
    T w;  // fourth component, 1 for points and 0 for directions when used with Matrix4
};

using Vector4I = Vector4<int>;
using Vector4F = Vector4<float>;
using Vector4D = Vector4<double>;

// Vector2: Two components of type T, usually window or texture coordinates.
template <typename T> struct Vector2 {

    // Components
    T u;  // horizontal component
    T v;  // vertical component
};

using Vector2I = Vector2<int>;
using Vector2F = Vector2<float>;
using Vector2D = Vector2<double>;

// Vector3: Three components of type T, used for positions and directions.
template <typename T> struct Vector3 {

    // Components
    T x;  // x component
    T y;  // y component
    T z;  // z component

    T vectorMagnitude() const;
    Vector3<T> vectorNormalized() const;
    Vector3<int> vectorToIntegersFloored() const;

    Vector3<T> operator+(const Vector3<T>& other) const;
    Vector3<T> operator-(const Vector3<T>& other) const;

    Vector3<T> operator+(T scalar) const;
    Vector3<T> operator-(T scalar) const;
    Vector3<T> operator*(T scalar) const;
    Vector3<T> operator/(T scalar) const;

    bool operator==(const Vector3<T>& other) const;
    bool operator<(const Vector3<T>& other) const;
};

using Vector3I = Vector3<int>;
using Vector3F = Vector3<float>;
using Vector3D = Vector3<double>;

template <typename T> T dotProduct(const Vector3<T>& a, const Vector3<T>& b);
template <typename T> Vector3<T> crossProduct(const Vector3<T>& a, const Vector3<T>& b);

/* Vector Magnitude:
/ -
Returns the vector's length. */
template <typename T> T Vector3<T>::vectorMagnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

/* Vector Normalized:
/ -
Returns this vector at unit length, or {0, 0, 0} if its length is below 1e-8. */
template <typename T> Vector3<T> Vector3<T>::vectorNormalized() const {
    T mag = vectorMagnitude();
    T eps = static_cast<T>(1e-8);
    if (mag < eps) return {0, 0, 0};
    
    return {x / mag, y / mag, z / mag};
}

/* Vector To Integers Floored:
/ -
Returns each component rounded toward negative infinity. */
template <typename T> Vector3<int> Vector3<T>::vectorToIntegersFloored() const {
    return Vector3<int>{
        static_cast<int>(std::floor(x)),
        static_cast<int>(std::floor(y)),
        static_cast<int>(std::floor(z))
    };
}

/* Operator + (Vector):
/ -
Adds each component. */
template <typename T> Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

/* Operator - (Vector):
/ -
Subtracts each component. */
template <typename T> Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

/* Operator + (Scalar):
/ -
Adds the scalar to each component. */
template <typename T> Vector3<T> Vector3<T>::operator+(T scalar) const {
    return {x + scalar, y + scalar, z + scalar};
}

/* Operator - (Scalar):
/ -
Subtracts the scalar from each component. */
template <typename T> Vector3<T> Vector3<T>::operator-(T scalar) const {
    return {x - scalar, y - scalar, z - scalar};
}

/* Operator * (Scalar):
/ -
Multiplies each component by the scalar. */
template <typename T> Vector3<T> Vector3<T>::operator*(T scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

/* Operator / (Scalar):
/ -
scalar != 0.
/ -
Divides each component by (scalar + 1e-8), so dividing by 0 does not produce infinity. */
template <typename T> Vector3<T> Vector3<T>::operator/(T scalar) const {
    T eps = static_cast<T>(1e-8);
    return {x / (scalar + eps), y / (scalar + eps), z / (scalar + eps)};
}

/* Operator ==:
/ -
True if every component is exactly equal. */
template <typename T> bool Vector3<T>::operator==(const Vector3<T>& other) const {
    return x == other.x && y == other.y && z == other.z;
}

/* Operator <:
/ -
Compares x, then y, then z. Lets Vector3 be a std::map key. */
template <typename T> bool Vector3<T>::operator<(const Vector3<T>& other) const {
    if (x != other.x) return x < other.x;
    if (y != other.y) return y < other.y;
    return z < other.z;
}

/* Dot Product:
/ -
a, b: any vectors. */
template <typename T> T dotProduct(const Vector3<T>& a, const Vector3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Cross Product:
/ -
a, b: any vectors.
/ -
Returns a vector perpendicular to both, following the right-hand rule. */
template <typename T> Vector3<T> crossProduct(const Vector3<T>& a, const Vector3<T>& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

/* Vector Remove Components:
/ -
vector_2: unit length.
/ -
Returns vector_1 with its component along vector_2 removed. */
template <typename T>
Vector3<T> vectorRemoveComponents(const Vector3<T>& vector_1, const Vector3<T>& vector_2) {
    T dot = dotProduct(vector_1, vector_2);
    return Vector3<T>{
        vector_1.x - vector_2.x * dot,
        vector_1.y - vector_2.y * dot,
        vector_1.z - vector_2.z * dot
    };
}

/* Vector Static Cast:
/ -
U: the component type to convert to. */
template <typename U, typename T>
Vector3<U> vectorStaticCast(const Vector3<T>& vector) {
    return Vector3<U>{
        static_cast<U>(vector.x),
        static_cast<U>(vector.y),
        static_cast<U>(vector.z)
    };
}
