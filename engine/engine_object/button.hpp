#pragma once

#include "../engine_platform/input.hpp"
#include "../engine_utility/vector.hpp"
#include "../engine_render/mesh.hpp"
#include "../engine_render/shader.hpp"
#include "../engine_render/draw.hpp"

#include <iostream>

// Button State: The user's current interaction with a button.
enum class ButtonState {
    Idle,
    Hover,
    Held,
    Activated
};

// Button Config: Size, colors, and opacity of a button.
struct ButtonConfig {
    
    // Size
    float height;  // half the button's height, in window UV units
    float width;  // half the button's width, in window UV units

    // Appearance
    Vector3F held_color;  // rgb ∈ [0.0f, 1.0f]
    Vector3F hover_color;  // rgb ∈ [0.0f, 1.0f]
    Vector3F idle_color;  // rgb ∈ [0.0f, 1.0f]
    float opacity = 1.0f;  // ∈ [0.0f, 1.0f]
};

// Textured Button Type: Size presets for textured buttons.
enum class TexturedButtonType {
    Square,
    Small,
    Large,
};

// Button
class Button {
public:
    Button() = default;
    Button(Vector2F position, ButtonConfig config);

    Primitive buildPrimitives();

    // Attributes
    ButtonState m_state = ButtonState::Idle;  // set each frame by processInput
    Vector2F m_position;  // center of the button, in window UV units
    ButtonConfig m_config;  // size, colors, and opacity

    bool isHover(const Input& input, float aspect);
    bool isHeld(const Input& input, float aspect);
    bool isReleased(const Input& input, float aspect);
    void processInput(const Input& input, float aspect);
};

// Textured Button: A button drawn with a texture from the font atlas.
class TexturedButton : public Button {
public:
    TexturedButton() = default;
    TexturedButton(Vector2F position, ButtonConfig style, TexturedButtonType type, float size);

    Primitive buildPrimitives();

    // Attributes
    TexturedButtonType m_type;  // which region of the font atlas to draw
    float m_size;  // meant to overwrite the height/width inside m_config, currently unused
};
