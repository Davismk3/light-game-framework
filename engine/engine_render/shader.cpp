#include "shader.hpp"

namespace {
    // Load Source: Returns the file's contents. Throws with the working directory and resolved path if the file cannot be opened. path: a file path, relative to the working directory or absolute.
    std::string loadSource(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file) {
            std::error_code cwd_error;
            std::filesystem::path cwd = std::filesystem::current_path(cwd_error);
            bool exists = std::filesystem::exists(path);
            throw std::runtime_error(
                "Failed to open shader: " + path.string() +
                "\nWorking directory: " + (cwd_error ? "<unavailable>" : cwd.string()) +
                "\nResolved to: " + std::filesystem::absolute(path).string() +
                "\nFile exists at that path: " + (exists ? "yes" : "no")
            );
        }
        std::stringstream stream;
        stream << file.rdbuf();

        return stream.str();
    }
}

// Shader Initialize: Compiles and links the program. Throws if compiling or linking fails. vertex_path, fragment_path: GLSL source files, relative to the working directory or absolute.
void Shader::shaderInitialize(const std::string& vertex_path, const std::string& fragment_path) {
    int success;
    char infoLog[512];
    
    std::string vertex_source = loadSource(vertex_path);
    std::string fragment_source = loadSource(fragment_path);

    // Vertex Shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_source_cstr = vertex_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_source_cstr, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);  // report compile errors if any
    if(!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error(std::string("Vertex shader compilation failed:\n") + infoLog);
    }

    // Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_source_cstr = fragment_source.c_str();
    glShaderSource(fragment_shader, 1, &fragment_source_cstr, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);  // report compile errors if any
    if(!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error(std::string("Fragment shader compilation failed:\n") + infoLog);
    }

    // Linking
    m_shader_id = glCreateProgram();
    glAttachShader(m_shader_id, vertex_shader);
    glAttachShader(m_shader_id, fragment_shader);
    glLinkProgram(m_shader_id);
    glGetProgramiv(m_shader_id, GL_LINK_STATUS, &success);  // report linking errors if any
    if(!success) {
        glGetProgramInfoLog(m_shader_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error(std::string("Shader program linking failed:\n") + infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

// Shader Use: Makes this program the active one.
void Shader::shaderUse() const {
    glUseProgram(m_shader_id);
}

// Shader Shutdown: Deletes the program, if any.
void Shader::shaderShutdown() {
    if (m_shader_id != 0) {
        glDeleteProgram(m_shader_id);
        m_shader_id = 0;
    }
}

// Shader Set Bool: name: a bool or int uniform in this program. Call shaderUse first.
void Shader::shaderSetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), (int)value);
}

// Shader Set Int: name: an int or sampler uniform in this program. Call shaderUse first.
void Shader::shaderSetInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), value); 
}

// Shader Set Float: name: a float uniform in this program. Call shaderUse first.
void Shader::shaderSetFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(m_shader_id, name.c_str()), value); 
} 

// Shader Set Vec3: name: a vec3 uniform in this program. Call shaderUse first.
void Shader::shaderSetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_shader_id, name.c_str()), x, y, z);
}

// Shader Set Mat4: The matrix is row-major, so it is transposed on upload. name: a mat4 uniform in this program. Call shaderUse first.
void Shader::shaderSetMat4(const std::string& name, const Matrix4F& matrix) const {
    glUniformMatrix4fv(
        glGetUniformLocation(m_shader_id, name.c_str()),
        1,
        GL_TRUE,
        matrix.matrixData()
    );
}

// Shader Id: Returns the OpenGL program id, 0 when not initialized.
unsigned int Shader::shaderId() const {
    return m_shader_id;
}

// ~Shader: Deletes the program, if any.
Shader::~Shader() {
    shaderShutdown();
}
