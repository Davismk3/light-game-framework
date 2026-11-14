#define STB_IMAGE_IMPLEMENTATION

#include "texture.hpp"

#include <utility>

// Texture (Move): other: left with no texture id, so its destructor deletes nothing.
Texture::Texture(Texture&& other) noexcept
    : m_texture_id(std::exchange(other.m_texture_id, 0)),
      m_width(std::exchange(other.m_width, 0)),
      m_height(std::exchange(other.m_height, 0)),
      m_channels(std::exchange(other.m_channels, 0)) {
}

// Operator = (Move): Deletes this texture before taking other's. other: left with no texture id, so its destructor deletes nothing.
Texture& Texture::operator=(Texture&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    textureShutdown();

    m_texture_id = std::exchange(other.m_texture_id, 0);
    m_width = std::exchange(other.m_width, 0);
    m_height = std::exchange(other.m_height, 0);
    m_channels = std::exchange(other.m_channels, 0);

    return *this;
}

// Texture Load: Loads the image flipped vertically, so v = 0 is the bottom of the image. Uses nearest filtering and clamps at the edges. Throws if the file cannot be loaded. path: an image file with 1, 3, or 4 channels, relative to the working directory or absolute.
void Texture::textureLoad(const std::string& path) {
    textureShutdown();

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!data) throw std::runtime_error("Failed to load texture: " + path);

    GLenum format = GL_RGB;
    if (m_channels == 4) format = GL_RGBA;
    else if (m_channels == 3) format = GL_RGB;
    else if (m_channels == 1) format = GL_RED;

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    // Mipmap 
    /*glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int mipmap_level = 0;

    glTexImage2D(GL_TEXTURE_2D, mipmap_level, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

// Texture Bind: Leaves unit i active, so later binds and loads also use unit i. i >= 0, the texture unit.
void Texture::textureBind(int i) const {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

// Texture Shutdown: Deletes the texture, if any.
void Texture::textureShutdown() {
    if (m_texture_id != 0) {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
}

// ~Texture: Deletes the texture, if any.
Texture::~Texture() {
    textureShutdown();
}
