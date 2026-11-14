#pragma once

#include "vector.hpp"
#include "quaternion.hpp"

// Matrix4: A 4x4 matrix used for 3D transforms.
template <typename T> struct Matrix4 {

    // Elements
    T m[4][4];  // row-major, m[row][column]

    // Construction
    static Matrix4<T> identityMatrix();
    static Matrix4<T> translationMatrix(const Vector3<T>& position);
    static Matrix4<T> scaleMatrix(const Vector3<T>& scale);
    static Matrix4<T> rotationMatrix(const Quaternion<T>& q);

    // Construction Graphics-Specific (USE FLOAT TYPES)
    static Matrix4<T> lookAtMatrix(const Vector3<T>& position, const Vector3<T>& forward, const Vector3<T>& up);
    static Matrix4<T> perspectiveMatrix(T field_of_view, T aspect_ratio, T near_plane, T far_plane);
    static Matrix4<T> orthographicMatrix(T left, T right, T bottom, T top, T near_plane, T far_plane);

    // Operators
    Matrix4<T> operator+(const Matrix4<T>& rhs) const;
    Matrix4<T> operator-(const Matrix4<T>& rhs) const;
    Matrix4<T> operator*(const Matrix4<T>& rhs) const;
    Vector4<T> operator*(const Vector4<T>& rhs) const;
    Matrix4<T> operator*(const T& scalar) const;
    Matrix4<T>& operator*=(const Matrix4<T>& rhs);

    // Utilities
    bool matrixIsIdentity() const;
    const T* matrixData() const;
    Vector4<T> matrixRow(int row) const;
    Vector4<T> matrixColumn(int column) const;
};

using Matrix4F = Matrix4<float>;
using Matrix4D = Matrix4<double>;

// Identity Matrix: Returns the identity matrix.
template<typename T> Matrix4<T> Matrix4<T>::identityMatrix() {
    Matrix4<T> matrix{};
    matrix.m[0][0] = 1;
    matrix.m[1][1] = 1;
    matrix.m[2][2] = 1;
    matrix.m[3][3] = 1;

    return matrix;
}

// Translation Matrix: position: the translation.
template<typename T> Matrix4<T> Matrix4<T>::translationMatrix(const Vector3<T>& position) {
    Matrix4<T> matrix = identityMatrix();
    matrix.m[0][3] = position.x;
    matrix.m[1][3] = position.y;
    matrix.m[2][3] = position.z;

    return matrix;
}

// Scale Matrix: scale: the scale along each axis.
template<typename T> Matrix4<T> Matrix4<T>::scaleMatrix(const Vector3<T>& scale) {
    Matrix4<T> matrix{};
    matrix.m[0][0] = scale.x;
    matrix.m[1][1] = scale.y;
    matrix.m[2][2] = scale.z;
    matrix.m[3][3] = 1;

    return matrix;
}

// Rotation Matrix: q: a unit quaternion.
template<typename T> Matrix4<T> Matrix4<T>::rotationMatrix(const Quaternion<T>& q) {
    Matrix4<T> matrix = identityMatrix();

    T ii = q.i * q.i;
    T jj = q.j * q.j;
    T kk = q.k * q.k;

    T ij = q.i * q.j;
    T ik = q.i * q.k;
    T jk = q.j * q.k;

    T wi = q.w * q.i;
    T wj = q.w * q.j;
    T wk = q.w * q.k;

    matrix.m[0][0] = 1.0 - 2.0 * (jj + kk);
    matrix.m[0][1] = 2.0 * (ij - wk);
    matrix.m[0][2] = 2.0 * (ik + wj);

    matrix.m[1][0] = 2.0 * (ij + wk);
    matrix.m[1][1] = 1.0 - 2.0 * (ii + kk);
    matrix.m[1][2] = 2.0 * (jk - wi);

    matrix.m[2][0] = 2.0 * (ik - wj);
    matrix.m[2][1] = 2.0 * (jk + wi);
    matrix.m[2][2] = 1.0 - 2.0 * (ii + jj);

    return matrix;
}

// Look At Matrix: Returns a view matrix that looks down -z. position: the camera's position. forward: the point to look at, not a direction. up: not parallel to forward - position.
template<typename T> Matrix4<T> Matrix4<T>::lookAtMatrix(const Vector3<T>& position, const Vector3<T>& forward, const Vector3<T>& up) {

    // [Rx, Ry, Rz,  0 ]  <- Matrix Made From Right, Up, Forward
    // [Ux, Uy, Uz,  0 ]
    // [Fx, Fy, Fz,  0 ]
    // [ 0,  0,  0,  1 ]
    // * 
    // [ 1,  0,  0, -Px]  <- Matrix Made From Camera Position
    // [ 0,  1,  0, -Py]
    // [ 0,  0,  1, -Pz]
    // [ 0,  0,  0,  1 ]
    // = 
    // Look At Matrix

    Matrix4<T> matrix{};

    Vector3<T> F = (forward - position).vectorNormalized();   // Forward Vector (From Position)
    Vector3<T> R = crossProduct(F, up).vectorNormalized();    // Right Vector (Constructed)
    Vector3<T> U = crossProduct(R, F).vectorNormalized();     // Up Vector

    matrix.m[0][0] = R.x;
    matrix.m[0][1] = R.y;
    matrix.m[0][2] = R.z;
    matrix.m[0][3] = dotProduct(R * -1.0, position);

    matrix.m[1][0] = U.x;
    matrix.m[1][1] = U.y;
    matrix.m[1][2] = U.z;
    matrix.m[1][3] = dotProduct(U * -1.0, position);
    
    matrix.m[2][0] = F.x * -1.0;
    matrix.m[2][1] = F.y * -1.0;
    matrix.m[2][2] = F.z * -1.0;
    matrix.m[2][3] = dotProduct(F, position);

    matrix.m[3][3] = 1.0;

    return matrix;
}

// Perspective Matrix: field_of_view ∈ (0, π), in radians. aspect_ratio > 0, width / height. 0 < near_plane < far_plane.
template<typename T> Matrix4<T> Matrix4<T>::perspectiveMatrix(T field_of_view, T aspect_ratio, T near_plane, T far_plane) {
    Matrix4<T> matrix{};
    
    T f = 1.0 / (std::tan(field_of_view / 2.0));

    matrix.m[0][0] = f / aspect_ratio;
    matrix.m[1][1] = f;
    matrix.m[2][2] = (far_plane + near_plane) / (near_plane - far_plane);
    matrix.m[2][3] = (2.0 * far_plane * near_plane) / (near_plane - far_plane);
    matrix.m[3][2] = -1.0;

    return matrix;
}

// Orthographic Matrix: left != right, bottom != top, near_plane != far_plane.
template<typename T> Matrix4<T> Matrix4<T>::orthographicMatrix(T left, T right, T bottom, T top, T near_plane, T far_plane) {
    Matrix4<T> matrix{};

    matrix.m[0][0] = 2.0 / (right - left);
    matrix.m[1][1] = 2.0 / (top - bottom);
    matrix.m[2][2] = -2.0 / (far_plane - near_plane);
    matrix.m[3][3] = 1.0;

    matrix.m[0][3] = -(right + left) / (right - left);
    matrix.m[1][3] = -(top + bottom) / (top - bottom);
    matrix.m[2][3] = -(far_plane + near_plane) / (far_plane - near_plane);

    return matrix;
}

// Operator +: Adds each element.
template<typename T> Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& rhs) const {
    Matrix4<T> result{};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            result.m[row][column] = m[row][column] + rhs.m[row][column];
        }
    }
    return result;
}

// Operator -: Subtracts each element.
template<typename T> Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& rhs) const {
    Matrix4<T> result{};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            result.m[row][column] = m[row][column] - rhs.m[row][column];
        }
    }
    return result;
}

// Operator * (Matrix): Returns the matrix product. (this * rhs) applies rhs first.
template<typename T> Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& rhs) const {
    Matrix4<T> result{};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            for (int i = 0; i < 4; ++i) {
                result.m[row][column] += m[row][i] * rhs.m[i][column];
            }
        }
    }
    return result;
}

// Operator * (Vector): Returns the transformed vector. rhs: w = 1 for points, 0 for directions.
template<typename T> Vector4<T> Matrix4<T>::operator*(const Vector4<T>& rhs) const {
    return Vector4<T>{
        m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * rhs.w,
        m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * rhs.w,
        m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * rhs.w,
        m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * rhs.w
    };
}

// Operator * (Scalar): Multiplies each element by the scalar.
template<typename T> Matrix4<T> Matrix4<T>::operator*(const T& scalar) const {
    Matrix4<T> result{};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            result.m[row][column] = m[row][column] * scalar;
        }
    }
    return result;
}

// Operator *=: Sets this to (this * rhs).
template<typename T> Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& rhs) {
    *this = *this * rhs;
    return *this;
}

// Matrix Is Identity: True if every element exactly matches the identity matrix.
template<typename T> bool Matrix4<T>::matrixIsIdentity() const {
    Matrix4<T> identity = identityMatrix();
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            if (m[row][column] != identity.m[row][column]) {
                return false;
            }
        }
    }
    return true;
}

// Matrix Data: Returns a pointer to the 16 elements, row-major.
template<typename T> const T* Matrix4<T>::matrixData() const {
    return &m[0][0];
}

// Matrix Row: row ∈ [0, 3].
template<typename T> Vector4<T> Matrix4<T>::matrixRow(int row) const {
    return Vector4<T>{
        m[row][0],
        m[row][1],
        m[row][2],
        m[row][3]
    };
}

// Matrix Column: column ∈ [0, 3].
template<typename T> Vector4<T> Matrix4<T>::matrixColumn(int column) const {
    return Vector4<T>{
        m[0][column],
        m[1][column],
        m[2][column],
        m[3][column]
    };
}
