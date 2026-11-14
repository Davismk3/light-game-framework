#include "primitive.hpp"

// Quad: quad returns a primitive for four vertices.
Primitive quad(
    const Vertex& vertex_a,
    const Vertex& vertex_b,
    const Vertex& vertex_c,
    const Vertex& vertex_d
) {
    return {{vertex_a, vertex_b, vertex_c, vertex_d}, {0, 1, 2, 0, 2, 3}};  // vertices and indices
}

// Append Primitive: Take another primitive and combine it with this one.
void Primitive::appendPrimitive(const Primitive& other) {
    const unsigned int base_index = static_cast<unsigned int>(vertices.size());

    vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());

    for (unsigned int index : other.indices) indices.push_back(base_index + index);
}

// Clear Primitive: Clear this primitive's vertices and indices.
void Primitive::clearPrimitive() {
    vertices.clear();
    indices.clear();
}
