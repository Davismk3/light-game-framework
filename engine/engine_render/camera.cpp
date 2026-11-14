#include "camera.hpp"

// Camera: Starts at the origin with no rotation and an identity projection.
Camera::Camera() {
    m_position = {0.0f, 0.0f, 0.0f};
    m_rotation = {1.0f, 0.0f, 0.0f, 0.0f};
    m_projection = Matrix4F::identityMatrix();
}

// Position: position: the camera's new position.
void Camera::position(const Vector3F& position) {
    m_position = position;
}

// Rotation: rotation: a unit quaternion.
void Camera::rotation(const QuaternionF& rotation) {
    m_rotation = rotation;
}

// Perspective: field_of_view ∈ (0, 180), in degrees. aspect_ratio > 0, the window's width / height. 0 < near_plane < far_plane.
void Camera::perspective(float field_of_view, float aspect_ratio, float near_plane, float far_plane) {
    field_of_view *= 3.1415926f / 180.0f;  // degrees to radians
    m_projection = Matrix4F::perspectiveMatrix(field_of_view, aspect_ratio, near_plane, far_plane);
}

// Orthographic: left != right, bottom != top, near_plane != far_plane.
void Camera::orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane) {
    m_projection = Matrix4F::orthographicMatrix(left, right, bottom, top, near_plane, far_plane);
}

// View Matrix: Returns the view matrix from m_position and m_rotation.
Matrix4F Camera::viewMatrix() const {
    Vector3F forward = m_rotation.quaternionRotateVector({0.0f, 0.0f, -1.0f});
    Vector3F up = m_rotation.quaternionRotateVector({0.0f, 1.0f, 0.0f});
    Vector3F target = m_position + forward;

    return Matrix4F::lookAtMatrix(m_position, target, up);
}

// Projection Matrix: Returns m_projection.
Matrix4F Camera::projectionMatrix() const {
    return m_projection;
}
