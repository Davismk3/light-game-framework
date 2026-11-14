#include "button.hpp"

// Button: position: the button's center, in window UV units. style: the button's size, colors, and opacity.
Button::Button(Vector2F position, ButtonConfig style) {
    m_position = position;
    m_config = style;
}

// Is Hover: Is the user's mouse hovering over this button? aspect > 0, the window's width / height.
bool Button::isHover(const Input& input, float aspect) {
    float mouse_u = input.inputMouseWindowU() * aspect;
    float mouse_v = input.inputMouseWindowV();

    const float u_min = m_position.u - m_config.width;
    const float u_max = m_position.u + m_config.width;
    const float v_min = m_position.v - m_config.height;
    const float v_max = m_position.v + m_config.height;

    return mouse_u >= u_min &&
           mouse_u <= u_max &&
           mouse_v >= v_min &&
           mouse_v <= v_max;
}

// Is Held: Is the user's mouse holding this button? aspect > 0, the window's width / height.
bool Button::isHeld(const Input& input, float aspect) {
    if (isHover(input, aspect) && input.inputMouseDown(MouseButton::left)) return true;
    return false;
}

// Is Released: Did the user's mouse release this button? aspect > 0, the window's width / height.
bool Button::isReleased(const Input& input, float aspect) {
    if (isHover(input, aspect) && input.inputMouseReleased(MouseButton::left)) return true;
    return false;
}

// Process Input: Update this button's state from the user's mouse. aspect > 0, the window's width / height.
void Button::processInput(const Input& input, float aspect) {
    if (isReleased(input, aspect)) m_state = ButtonState::Activated;
    if (isHeld(input, aspect)) m_state = ButtonState::Held;
    else if (isHover(input, aspect)) m_state = ButtonState::Hover;
    else m_state = ButtonState::Idle;
}

// Build Primitives: Returns a single untextured quad covering this button.
Primitive Button::buildPrimitives() {
    const float left = m_position.u - m_config.width;
    const float right = m_position.u + m_config.width;
    const float bottom = m_position.v - m_config.height;
    const float top = m_position.v + m_config.height;

    Primitive button_primitive = quad(
        {right, top, 0.0f, 1.0f, 1.0f},
        {right, bottom, 0.0f, 1.0f, 0.0f},
        {left, bottom, 0.0f, 0.0f, 0.0f},
        {left, top, 0.0f, 0.0f, 1.0f}
    );
    return button_primitive;
}

// Textured Button: position: the button's center, in window UV units. style: the button's size, colors, and opacity. type: Square, Small, or Large. size: overwrites the height/width inside style.
TexturedButton::TexturedButton(Vector2F position, ButtonConfig style, TexturedButtonType type, float size) {
    m_position = position;
    m_config = style;
    m_type = type;
    m_size = size;
}

// Build Primitives (Textured): Returns a quad covering this button, textured with the font atlas region for its type.
Primitive TexturedButton::buildPrimitives() {
    Primitive texture_button_primitive;

    if (m_type == TexturedButtonType::Large) {
        const float l = m_position.u - m_config.width;
        const float r = m_position.u + m_config.width;
        const float b = m_position.v - m_config.height;
        const float t = m_position.v + m_config.height;

        float u0 = 0.0f / FONT_AXIS_WIDTH;
        float v0 = 1.0f - 58.0f / FONT_AXIS_HEIGHT;
        float u1 = 120.0f / FONT_AXIS_WIDTH;
        float v1 = 1.0f - 70.0f / FONT_AXIS_HEIGHT;

        texture_button_primitive.appendPrimitive(quad(
            {r, t, 0.0f, u1, v0},
            {r, b, 0.0f, u1, v1},
            {l, b, 0.0f, u0, v1},
            {l, t, 0.0f, u0, v0}
        ));
    } else if (m_type == TexturedButtonType::Small) {
        const float l = m_position.u - m_config.width;
        const float r = m_position.u + m_config.width;
        const float b = m_position.v - m_config.height;
        const float t = m_position.v + m_config.height;

        float u0 = 0.0f / FONT_AXIS_WIDTH;
        float v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT;
        float u1 = 58.0f / FONT_AXIS_WIDTH;
        float v1 = 1.0f - 83.0f / FONT_AXIS_HEIGHT;

        texture_button_primitive.appendPrimitive(quad(
            {r, t, 0.0f, u1, v0},
            {r, b, 0.0f, u1, v1},
            {l, b, 0.0f, u0, v1},
            {l, t, 0.0f, u0, v0}
        ));
    } else if (m_type == TexturedButtonType::Square) {
        const float l = m_position.u - m_config.width;
        const float r = m_position.u + m_config.width;
        const float b = m_position.v - m_config.height;
        const float t = m_position.v + m_config.height;

        float u0 = 60.0f / FONT_AXIS_WIDTH;
        float v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT;
        float u1 = 71.0f / FONT_AXIS_WIDTH;
        float v1 = 1.0f - 83.0f / FONT_AXIS_HEIGHT;

        texture_button_primitive.appendPrimitive(quad(
            {r, t, 0.0f, u1, v0},
            {r, b, 0.0f, u1, v1},
            {l, b, 0.0f, u0, v1},
            {l, t, 0.0f, u0, v0}
        ));
    }

    return texture_button_primitive;
}
