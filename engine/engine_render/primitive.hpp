#pragma once

#include <cstdint>
#include <vector>

/*
Bitwise Operations:
"|" - Bitwise OR,       0011 | 0100 = 0111 (addition)
"&" - Bitwise AND,      1011 & 0110 = 0010 (multiplication)
"~" - Bitwise NOT,      ~1100 = 0011
"^" - Bitwise XOR,      0011 ^ 0101 = 0110

Bitwise:
"0xFu" - means binary 1111

// Pack:
packed |= (value & mask) << shift;

// Unpack:
value = (packed >> shift) & mask;
*/

// Vertex [24 Bytes]
struct Vertex {

    // Core Position & Texture Storage [12 + 4 Bytes]
    float x, y, z;  // position in the mesh's local space
    _Float16 u, v;  // ∈ [0.0f, 1.0f]

    // Extra Data Storage [4 + 4 Bytes]
    // You set this yourself!
    std::uint32_t data1 = 0x00000AAAu;  // initialized to 10 where there is 'A'
    std::uint32_t data2 = 0x0000AAA0u;  // initialized to 10 where there is 'A'

    // Constructors
    Vertex() noexcept = default;
    Vertex(float x_, float y_, float z_, float u_, float v_) noexcept : x(x_), y(y_), z(z_), u(static_cast<_Float16>(u_)), v(static_cast<_Float16>(v_)) {}
};

// Primitive: A packet of vertices and indices sent to the GPU.
struct Primitive {

    // Geometry Storage
    std::vector<Vertex> vertices;  // every corner of every shape in this primitive
    std::vector<unsigned int> indices;  // offsets into vertices, three per triangle
    
    void appendPrimitive(const Primitive& other_primitive);
    void clearPrimitive();
};

Primitive quad(
    const Vertex& vertex_a,
    const Vertex& vertex_b,
    const Vertex& vertex_c,
    const Vertex& vertex_d
);
