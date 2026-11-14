#pragma once

#include "primitive.hpp"

#include <utility>
#include <stdexcept>
#include <glad/gl.h>

// Mesh: Vertices and indices uploaded to the GPU and drawn through one VAO.
class Mesh {
public:
    Mesh() = default;  // needed since we define additional constructors below
    ~Mesh();
    
    void meshClear();
    void meshAppendPrimitives(const Primitive& primitive);
    void meshUpload();  // Upload To GPU
    void meshShutdown();

    Primitive meshGetPrimitives();
    
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    
    // CPU Geometry
    std::vector<Vertex> m_vertices;  // emptied by meshUpload
    std::vector<unsigned int> m_indices;  // emptied by meshUpload

    // Counts
    unsigned int m_vertex_count = 0;  // set by meshUpload
    unsigned int m_index_count = 0;  // set by meshUpload, 0 draws without indices

    // GPU Handles
    unsigned int m_VAO = 0;  // 0 when not uploaded
    unsigned int m_VBO = 0;  // 0 when not uploaded
    unsigned int m_EBO = 0;  // 0 when not uploaded or there are no indices

private:

    // unsigned int m_draw_mode = GL_TRIANGLES;

};
