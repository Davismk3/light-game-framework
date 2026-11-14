#include "mesh.hpp"

// Mesh (Move): other: left with no GPU handles, so its destructor deletes nothing.
Mesh::Mesh(Mesh&& other) noexcept
    : m_vertices(std::move(other.m_vertices)),
      m_indices(std::move(other.m_indices)),
      m_vertex_count(std::exchange(other.m_vertex_count, 0)),  // missing line; bug found by Claude Code
      m_index_count(std::exchange(other.m_index_count, 0)),  // missing line; bug found by Claude Code
      m_VAO(std::exchange(other.m_VAO, 0)),
      m_VBO(std::exchange(other.m_VBO, 0)),
      m_EBO(std::exchange(other.m_EBO, 0)) {
}

// Operator = (Move): Deletes this mesh's GPU resources before taking other's. other: left with no GPU handles, so its destructor deletes nothing.
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    meshShutdown();

    m_vertices = std::move(other.m_vertices);  // missing line; bug found by Claude Code
    m_indices = std::move(other.m_indices);  // missing line; bug found by Claude Code

    m_vertex_count = std::exchange(other.m_vertex_count, 0);
    m_index_count = std::exchange(other.m_index_count, 0);

    m_VAO = std::exchange(other.m_VAO, 0);
    m_VBO = std::exchange(other.m_VBO, 0);
    m_EBO = std::exchange(other.m_EBO, 0);

    return *this;
}

// Mesh Clear: Clears the CPU vertices and indices. The uploaded GPU mesh is unchanged.
void Mesh::meshClear() {
    m_vertices.clear();
    m_indices.clear();
}

// Mesh Append Primitives: Appends the primitive's vertices and indices, shifting its indices past the existing vertices. primitive: indices are offsets into its own vertices.
void Mesh::meshAppendPrimitives(const Primitive& primitive) {
    unsigned int base_index = static_cast<unsigned int>(m_vertices.size());
    
    for (const Vertex& vertex : primitive.vertices) m_vertices.push_back(vertex);
    for (const int index : primitive.indices) m_indices.push_back(base_index + index);
}

// Mesh Get Primitives: Returns the CPU vertices and indices as one primitive. Empty after meshUpload.
Primitive Mesh::meshGetPrimitives() {
    return Primitive{m_vertices, m_indices };
}

// Mesh Upload: Uploads the vertices and indices to the GPU, sets the vertex layout, then empties the CPU copies.
void Mesh::meshUpload() {
    meshShutdown();  // Avoid Memory Leak

    m_vertex_count = static_cast<unsigned int>(m_vertices.size());
    m_index_count  = static_cast<unsigned int>(m_indices.size());

    // Vertex Array Object
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO); 

    // Buffer Objects
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
    if (m_index_count > 0) {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
    }

    // ---

    // This is how the CPU tells the Shader/GPU what the vertex values are:

    // Position (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture (u,v)
    glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, u)));
    glEnableVertexAttribArray(1);

    // Data 1
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, data1)));
    glEnableVertexAttribArray(2);

    // Data 2
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, data2)));
    glEnableVertexAttribArray(3);

    // ---

    // Clear CPU
    std::vector<Vertex>().swap(m_vertices);  // swap for empty 
    std::vector<unsigned int>().swap(m_indices);  // swap for empty 

    glBindVertexArray(0);
}

// Mesh Shutdown: Deletes the GPU resources, if any.
void Mesh::meshShutdown() {
    if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO); m_VAO = 0;
    if (m_VBO != 0) glDeleteBuffers(1, &m_VBO); m_VBO = 0;
    if (m_EBO != 0) glDeleteBuffers(1, &m_EBO); m_EBO = 0;
}

// ~Mesh: Deletes the GPU resources, if any.
Mesh::~Mesh() {
    meshShutdown();
}
