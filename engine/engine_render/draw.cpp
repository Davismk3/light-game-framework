#include "draw.hpp"

// Draw Mesh: Uses the shader and draws the whole mesh, with indices if it has any.
void drawMesh(Mesh& mesh, Shader& shader) {
    shader.shaderUse();
    
    // Bind VAO
    glBindVertexArray(mesh.m_VAO);

    // Draw
    if (mesh.m_index_count > 0) glDrawElements(GL_TRIANGLES, mesh.m_index_count, GL_UNSIGNED_INT, nullptr);
    else glDrawArrays(GL_TRIANGLES, 0, mesh.m_vertex_count);

    // Unbind VAO
    glBindVertexArray(0);
}

// Draw Mesh Range: Separate draw calls on a single mesh, which allows local shader differences within the mesh. first_index + index_count <= the mesh's index count.
void drawMeshRange(
    Mesh& mesh,
    Shader& shader,
    unsigned int first_index,
    unsigned int index_count
) {
    shader.shaderUse();

    // Bind VAO
    glBindVertexArray(mesh.m_VAO);

    // Draw
    const std::size_t byte_offset = static_cast<std::size_t>(first_index) * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, reinterpret_cast<const void*>(byte_offset));

    // Unbind VAO
    glBindVertexArray(0);
}

// Draw Mesh Range Efficient: Currently unused. index_size: how many draw calls, each starting 6 indices after the last. index_count: how many indices each draw call uses.
void drawMeshRangeEfficient(
    Mesh& mesh,
    Shader& shader,
    unsigned int index_size,
    unsigned int index_count
) {
    shader.shaderUse();

    // Bind VAO
    glBindVertexArray(mesh.m_VAO);

    for (int i = 0; i < index_size; i++) {
        const std::size_t byte_offset = static_cast<std::size_t>(static_cast<unsigned int>(i) * 6) * sizeof(unsigned int);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, reinterpret_cast<const void*>(byte_offset));
    }

    // Unbind VAO
    glBindVertexArray(0);
}

// Draw Mesh Range Exposed: Like drawMeshRange, but does not use the shader or bind the VAO. The caller must do both first. first_index + index_count <= the mesh's index count.
void drawMeshRangeExposed(
    Mesh& mesh,
    Shader& shader,
    unsigned int first_index,
    unsigned int index_count
) {

    // Draw
    const std::size_t byte_offset = static_cast<std::size_t>(first_index) * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, reinterpret_cast<const void*>(byte_offset));
}

// Draw Clear: Clears the color and depth buffers to this color. red, green, blue, alpha ∈ [0.0f, 1.0f].
void drawClear(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
