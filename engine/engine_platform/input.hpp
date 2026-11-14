#pragma once

#include "window.hpp"

#include <array>
#include <GLFW/glfw3.h>

// Mouse Button: The mouse buttons Input tracks.
enum class MouseButton {
    left,
    middle,
    right,
    COUNT
};

// Key: The keyboard keys Input tracks. COUNT must stay last.
enum class Key {
    A,
    B, 
    C, 
    D, 
    E, 
    F, 
    G,
    H, 
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    Escape,
    Space,
    LShift,
    RShift,
    Delete,

    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Zero,

    Period,
    SemiColon,
    Comma,
    RSlash,
    LSlash,
    QuotationMarkSingle,
    Minus,
    Equal,
    LSquareBracket,
    RSquareBracket,
    BackTick,

    Command,

    COUNT
};

// Input: Tracks keyboard and mouse state from a window, updated once per frame.
class Input {
public:
    void inputAttachWindow(Window& window);

    void inputUpdate();

    bool inputKeyDown(Key key) const;
    bool inputKeyPressed(Key key) const;
    bool inputKeyReleased(Key key) const;

    bool inputMouseDown(MouseButton mouse_button) const;
    bool inputMousePressed(MouseButton mouse_button) const;
    bool inputMouseReleased(MouseButton mouse_button) const;
    
    float inputMouseDeltaU() const;
    float inputMouseDeltaV() const;

    float inputMouseWindowU() const;
    float inputMouseWindowV() const;

    void inputSetMouseCaptured(bool captured);
    bool inputMouseCaptured() const;

    float getMouseScroll();

private:

    // Window
    GLFWwindow* m_window = nullptr;  // initialize as null pointer

    // Counts
    static constexpr int KEY_COUNT = static_cast<int>(Key::COUNT);  // number of tracked keys
    static constexpr int MOUSE_BUTTON_COUNT = static_cast<int>(MouseButton::COUNT);  // number of tracked mouse buttons

    // Keys
    std::array<bool, KEY_COUNT> m_current_keys = {};  // down this frame, indexed by Key
    std::array<bool, KEY_COUNT> m_previous_keys = {};  // down last frame, indexed by Key

    // Mouse Buttons
    std::array<bool, MOUSE_BUTTON_COUNT> m_current_mouse_buttons;  // down this frame, indexed by MouseButton
    std::array<bool, MOUSE_BUTTON_COUNT> m_previous_mouse_buttons;  // down last frame, indexed by MouseButton

    // Mouse Position
    double m_mouse_u = 0.0;  // pixels from the window's left edge
    double m_mouse_v = 0.0;  // pixels from the window's top edge
    double m_previous_mouse_u = 0.0;  // m_mouse_u last frame
    double m_previous_mouse_v = 0.0;  // m_mouse_v last frame

    // Mouse Delta
    float m_mouse_delta_u = 0.0f;  // pixels moved right since last frame
    float m_mouse_delta_v = 0.0f;  // pixels moved down since last frame

    // Flags
    bool m_has_previous_mouse_position = false;  // false until the first update, and after capture changes, so the delta does not jump
    bool m_mouse_captured = false;  // true while the cursor is hidden and locked to the window
};
