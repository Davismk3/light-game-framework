#pragma once

#include "../engine_utility/matrix.hpp"

#include <string>
#include <glad/gl.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

// Color: Currently unused.
enum class Color {
    red,
    blue,
    green
};

// Shader: A compiled and linked vertex and fragment shader program.
class Shader {
public:
    void shaderInitialize(const std::string& vertex_path, const std::string& fragment_path);
    void shaderUse() const;
    void shaderShutdown();

    void shaderSetBool(const std::string &name, bool value) const;  
    void shaderSetInt(const std::string &name, int value) const;  
    
    // Options: "u_light", "u_opacity", "u_alpha_cutoff", "a_rotation_speed", "a_initial_angle"
    void shaderSetFloat(const std::string &name, float value) const;
    
    // Options: "u_tint", "a_normal", "a_center", "a_shift"
    void shaderSetVec3(const std::string& name, float x, float y, float z) const;

    // Options: "u_projection", "u_view", "u_model"
    void shaderSetMat4(const std::string& name, const Matrix4F& matrix) const;

    unsigned int shaderId() const;

    // Disables Copying
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader() = default;
    ~Shader();  // deletes GPU resource
    
private:

    // Program
    unsigned int m_shader_id = 0;  // OpenGL program id, 0 when not initialized
};
