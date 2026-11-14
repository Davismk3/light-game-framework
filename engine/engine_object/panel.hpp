#pragma once

#include "../engine_config.hpp"
#include "../engine_utility/vector.hpp"
#include "../engine_render/primitive.hpp"

// Panel Config: Size, position, and appearance of a panel.
struct PanelConfig {

    // Size
    float pixel_size = 1.0f;  // scales the border thickness
    float width = 0.1f;  // half the center's width, in window UV units
    float height = 0.1f;  // half the center's height, in window UV units

    // Position
    Vector2F center_position = {0.0f, 0.0f};  // in window UV units

    // Appearance
    float opacity = 1.0f;  // ∈ [0.0f, 1.0f]
    Vector3F tint = {1.0f, 1.0f, 1.0f};  // rgb ∈ [0.0f, 1.0f]
    bool follows_cursor = false;  // shifts the panel by the mouse position
    bool inverted = false;  // currently unused
};

// Panel: A bunch of textured quads organized to form a display panel, like an inventory or hotbar backdrop.
struct Panel {
    Primitive buildPrimitives();

    // Config
    PanelConfig m_panel_config;  // size, position, and appearance
};
