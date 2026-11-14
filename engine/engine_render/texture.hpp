#pragma once

#include "stb_image.h"

#include <string>
#include <glad/gl.h>
#include <stdexcept>

// Texture: An image loaded onto the GPU.
class Texture {
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    
    void textureLoad(const std::string& path);
    void textureBind(int i = 0) const;
    void textureShutdown();

private:

    // Handle
    unsigned int m_texture_id = 0;  // OpenGL texture id, 0 when not loaded

    // Image
    int m_width = 0;  // in pixels
    int m_height = 0;  // in pixels
    int m_channels = 0;  // 1, 3, or 4, read from the image file
};
