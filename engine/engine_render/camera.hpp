#pragma once

#include "../engine_utility/quaternion.hpp"
#include "../engine_utility/vector.hpp"
#include "../engine_utility/matrix.hpp"

// Camera: A position, rotation, and projection used to build the view and projection matrices.
class Camera {
public:
    Camera();

    void position(const Vector3F& position);
    void rotation(const QuaternionF& rotation);
    
    void perspective(float field_of_view, float aspect_ratio, float near_plane, float far_plane);
    void orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);

    Matrix4F viewMatrix() const;
    Matrix4F projectionMatrix() const;

    // Transform
    Vector3F m_position;  // the eye of the view matrix
    QuaternionF m_rotation;  // (w, i, j, k), identity looks down -z with +y up

    // Projection
    Matrix4F m_projection;  // set by perspective or orthographic

private:

};
