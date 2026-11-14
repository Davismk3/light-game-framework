#include "screen_simulation.hpp"

void SimulationScreen::initialize() {
    std::cout << "at simulation\n";

    // Place Additional Simulation Initialization Logic Here:

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

    m_menu.addText(Text({0.0f, 0.8f}, text_style, "Screen 2"));
    text_style.size = 0.05f;
    m_menu.addText(Text({0.0f, 0.0f}, text_style, "Click Me!"));

    m_menu.initialize();
    m_menu.buildMesh();

    // -
}

void SimulationScreen::shutdown(ControlsState& controls_state) {
    controls_state.simulation_to_title = false;

    // Place Additional Simulation Shutdown Logic Here:

    // -
}

void SimulationScreen::processInput(Input& input, ControlsState& controls_state) {
    
    // Place Additional Simulation Input Logic Here:

    m_menu.processInput(input);
    if (m_menu.m_texture_buttons[0].m_state == ButtonState::Hover && input.inputMouseReleased(MouseButton::left)) {
        controls_state.simulation_to_title = true;
    }

    // -
}

void SimulationScreen::update(float& delta_time) {

    // Place Additional Simulation Update Logic Here:

    // -
}

void SimulationScreen::render() {

    // Place Additional Simulation Render Logic Here:

    drawClear(0.2f, 0.1f, 0.0f, 1.0f);
    m_menu.draw();

    // -
}
