#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "png.hpp"

// Save PNG: Creates the parent directories if needed. Throws if the buffer size is wrong or the write fails. path: a .png path, relative to the working directory or absolute. width, height > 0, in pixels. rgba: width * height * 4 bytes, rows from the top.
void savePNG(const std::string& path, int width, int height, const std::vector<std::uint8_t>& rgba) {
    constexpr int channels = 4;

    if (rgba.size() != static_cast<std::size_t>(width * height * channels)) {
        throw std::runtime_error("PNG pixel-buffer size is incorrect");
    }

    std::filesystem::create_directories(std::filesystem::path(path).parent_path());

    if (!stbi_write_png(path.c_str(), width, height, channels, rgba.data(), width * channels)) {
        throw std::runtime_error("Failed to save PNG: " + path);
    }
}
