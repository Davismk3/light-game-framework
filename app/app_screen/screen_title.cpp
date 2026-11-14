#include "screen_title.hpp"

void TitleScreen::initialize() {
    std::cout << "at title\n";

    // Place Additional Title Initialization Logic Here:

    Vector2F position = {0.0f, 0.0f};
    ButtonConfig style;
    style.height = 0.06f;
    style.width = 0.4f;
    style.held_color = {0.7f, 1.0f, 0.7f};
    style.hover_color = {0.85f, 0.85f, 0.85f};
    style.idle_color = {1.0f, 1.0f, 1.0f};
    style.opacity = 1.0f;

    m_menu.addTexturedButton(TexturedButton(position, style, TexturedButtonType::Large, 1.0f));

    TextConfig text_style;
    text_style.size = 0.1f;
    text_style.color = {0.7f, 0.7f, 1.0f};
    text_style.positioning = TextPositioning::Centered;
    text_style.has_shadow = true;

    m_menu.addText(Text({0.0f, 0.8f}, text_style, "Screen 1"));
    text_style.size = 0.05f;
    m_menu.addText(Text({0.0f, 0.0f}, text_style, "Click Me!"));

    m_menu.initialize();
    m_menu.buildMesh();

    // -
}

void TitleScreen::shutdown(ControlsState& controls_state) {
    controls_state.title_to_simulation = false;

    // Place Additional Title Shutdown Logic Here:

    // -
}

void TitleScreen::processInput(Input& input, ControlsState& controls_state) {

    // Place Additional Title Input Logic Here:

    m_menu.processInput(input);
    if (m_menu.m_texture_buttons[0].m_state == ButtonState::Hover && input.inputMouseReleased(MouseButton::left)) {
        controls_state.title_to_simulation = true;
    }

    // -
}

void TitleScreen::update(float& delta_time) {

    // Place Additional Title Update Logic Here:

    // -
}

void TitleScreen::render() {
    
    // Place Additional Title Render Logic Here:

    drawClear(0.0f, 0.1f, 0.2f, 1.0f);
    m_menu.draw();

    // -
}
