#pragma once

#include "../engine_utility/vector.hpp"
#include "../engine_render/primitive.hpp"
#include "../engine_render/texture.hpp"
#include "../engine_platform/input.hpp"
#include "../engine_config.hpp"

#include <string>
#include <iostream>
#include <map>

/*
This script contains Text and Textbox objects. 
*/

// Text Positioning: Whether the text's start position is to the left, right, or center of the string.
enum class TextPositioning {
    LeftToRight,
    RightToLeft,
    Centered,
};

// Text Box State: The user's current interaction with a textbox.
enum class TextBoxState {
    Idle, 
    Hover,
    Active
};

// Glyph Atlas Position UV: A glyph's upper left pixel in the font atlas.
struct GlyphAtlasPositionUV {
    
    // Pixel
    int u;  // pixels from the atlas's left edge
    int v;  // pixels from the atlas's top edge
};

// Text Config: Size, colors, background, opacity, and positioning of text.
struct TextConfig {

    // Attributes
    float size;  // glyph height, in window UV units
    Vector3F color = {0.0f, 0.0f, 0.0f};  // rgb ∈ [0.0f, 1.0f]
    TextPositioning positioning = TextPositioning::LeftToRight;  // which part of the string the start position marks
    Vector3F shadow_color = {0.0f, 0.0f, 0.0f};  // rgb ∈ [0.0f, 1.0f]
    bool has_shadow = false;  // draws a shadow one glyph pixel down and to the right
    bool has_background = false;  // draws an untextured quad behind the string
    Vector3F background_color = {0.0f, 0.0f, 0.0f};  // rgb ∈ [0.0f, 1.0f]
    float background_opacity = 1.0f;  // ∈ [0.0f, 1.0f]
    float opacity = 1.0f;  // ∈ [0.0f, 1.0f]
};

// Text Box Config: Size, colors, and opacity of a textbox.
struct TextBoxConfig {

    // Size
    float height;  // half the box's height, in window UV units
    float width;  // half the box's width, in window UV units

    // Appearance
    Vector3F hover_color;  // rgb ∈ [0.0f, 1.0f], also used while typing
    Vector3F idle_color;  // rgb ∈ [0.0f, 1.0f]
    Vector3F typing_color;  // currently unused
    float opacity = 1.0f;  // ∈ [0.0f, 1.0f]
};

// Text: A string drawn from the font atlas.
struct Text {
    Text() = default;
    Text(Vector2F start_position, TextConfig style, std::string string);

    // String
    std::string m_string;  // stored as typed, special characters included
    char m_special_init = '\\';  // starts a special character, e.g. "\1"
    float m_string_length = 0.0f;  // drawn width in window UV units, set by buildPrimitives

    // Position
    Vector2F m_start_position;  // normalized to ([-1, 1], [-1, 1]) for screen
    Vector2F m_end_position = m_start_position;  // normalized to ([-1, 1], [-1, 1]) for screen, moved forward by buildPrimitives

    // Config
    TextConfig m_config;  // size, colors, background, opacity, and positioning

    // Flags
    bool allows_special_characters = false;  // currently unused
    bool follows_cursor = false;  // shifts the text by the mouse position

    Primitive buildPrimitives();
};

// Text Box: A box the user can click and type text into.
class TextBox {
public:
    TextBox() = default;
    void initializeDefault();
    void processInput(const Input& input, float aspect);
    void addChar(char char_to_add);
    void subtractChar();

    bool isHover(const Input& input, float aspect);

    Primitive buildBoxPrimitives();
    Primitive buildTextPrimitives();
    Primitive buildCursorPrimitives();

    // Flags
    bool m_is_active = false;  // set on click, true while the user is typing
    bool m_text_changed = false;  // true for the frame the string changes
    bool m_state_just_changed = false;  // true for the frame the user clicks

    // Attributes
    Vector2F m_position;  // center of the box, in window UV units
    TextBoxConfig m_config;  // size, colors, and opacity

    // Texts
    Text m_textbox_empty_text;  // placeholder shown while empty and inactive
    Text m_textbox_text;  // the user's typed string
    Text m_textbox_cursor;  // the "<" cursor
    Text m_combined_current_text;  // whichever of the empty or typed text is drawn

    // State
    TextBoxState m_state = TextBoxState::Idle;  // set each frame by processInput

    // Max Length
    bool m_string_at_max_length = false;  // blocks typing once the text nearly fills the box
};

// Map for glyph to glyph ID.
inline std::map<char, int> Glyph_To_GlyphId_Map {
    {'A', 1},
    {'B', 2},
    {'C', 3},
    {'D', 4},
    {'E', 5},
    {'F', 6},
    {'G', 7},
    {'H', 8},
    {'I', 9},
    {'J', 10},
    {'K', 11},
    {'L', 12},
    {'M', 13},
    {'N', 14},
    {'O', 15},
    {'P', 16},
    {'Q', 17},
    {'R', 18},
    {'S', 19},
    {'T', 20},
    {'U', 21},
    {'V', 22},
    {'W', 23},
    {'X', 24},
    {'Y', 25},
    {'Z', 26},
    {'a', 27},
    {'b', 28},
    {'c', 29},
    {'d', 30},
    {'e', 31},
    {'f', 32},
    {'g', 33},
    {'h', 34},
    {'i', 35},
    {'j', 36},
    {'k', 37},
    {'l', 38},
    {'m', 39},
    {'n', 40},
    {'o', 41},
    {'p', 42},
    {'q', 43},
    {'r', 44},
    {'s', 45},
    {'t', 46},
    {'u', 47},
    {'v', 48},
    {'w', 49},
    {'x', 50},
    {'y', 51},
    {'z', 52},
    {'1', 53},
    {'2', 54},
    {'3', 55},
    {'4', 56},
    {'5', 57},
    {'6', 58},
    {'7', 59},
    {'8', 60},
    {'9', 61},
    {'0', 62},
    {'.', 63},
    {'!', 64},
    {'?', 65},
    {':', 66},
    {';', 67},
    {',', 68},
    {'_', 69},
    {'/', 70},
    {'\\', 71},
    {'"', 72},
    {'\'', 73},
    {' ', 74},
    {'|', 75},
    {'@', 76},
    {'#', 77},
    {'$', 78},
    {'%', 79},
    {'^', 80},
    {'&', 81},
    {'*', 82},
    {'(', 83},
    {')', 84},
    {'-', 85},
    {'+', 86},
    {'=', 87},
    {'{', 88},
    {'}', 89},
    {'[', 90},
    {']', 91},
    {'`', 92},
    {'<', 93},
    {'>', 94},
    {'~', 95},
};

// Map for key to char.
inline std::map<Key, char> Key_To_Char_Map {
    {Key::A, 'a'},
    {Key::B, 'b'},
    {Key::C, 'c'},
    {Key::D, 'd'},
    {Key::E, 'e'},
    {Key::F, 'f'},
    {Key::G, 'g'},
    {Key::H, 'h'},
    {Key::I, 'i'},
    {Key::J, 'j'},
    {Key::K, 'k'},
    {Key::L, 'l'},
    {Key::M, 'm'},
    {Key::N, 'n'},
    {Key::O, 'o'},
    {Key::P, 'p'},
    {Key::Q, 'q'},
    {Key::R, 'r'},
    {Key::S, 's'},
    {Key::T, 't'},
    {Key::U, 'u'},
    {Key::V, 'v'},
    {Key::W, 'w'},
    {Key::X, 'x'},
    {Key::Y, 'y'},
    {Key::Z, 'z'},
    {Key::Space, ' '},
    {Key::One, '1'},
    {Key::Two, '2'},
    {Key::Three, '3'},
    {Key::Four, '4'},
    {Key::Five, '5'},
    {Key::Six, '6'},
    {Key::Seven, '7'},
    {Key::Eight, '8'},
    {Key::Nine, '9'},
    {Key::Zero, '0'},
    {Key::Period, '.'},
    {Key::SemiColon, ';'},
    {Key::Comma, ','},
    {Key::RSlash, '/'},
    {Key::LSlash, '\\'},
    {Key::QuotationMarkSingle, '\''},
    {Key::Minus, '-'},
    {Key::Equal, '='},
    {Key::LSquareBracket, '['},
    {Key::RSquareBracket, ']'},
    {Key::BackTick, '`'},
};

// Map for shifted key to char.
inline std::map<Key, char> ShiftedKey_To_Char_Map {
    {Key::A, 'A'},
    {Key::B, 'B'},
    {Key::C, 'C'},
    {Key::D, 'D'},
    {Key::E, 'E'},
    {Key::F, 'F'},
    {Key::G, 'G'},
    {Key::H, 'H'},
    {Key::I, 'I'},
    {Key::J, 'J'},
    {Key::K, 'K'},
    {Key::L, 'L'},
    {Key::M, 'M'},
    {Key::N, 'N'},
    {Key::O, 'O'},
    {Key::P, 'P'},
    {Key::Q, 'Q'},
    {Key::R, 'R'},
    {Key::S, 'S'},
    {Key::T, 'T'},
    {Key::U, 'U'},
    {Key::V, 'V'},
    {Key::W, 'W'},
    {Key::X, 'X'},
    {Key::Y, 'Y'},
    {Key::Z, 'Z'},
    {Key::Space, ' '},
    {Key::One, '!'},
    {Key::Two, '@'},
    {Key::Three, '#'},
    {Key::Four, '$'},
    {Key::Five, '%'},
    {Key::Six, '^'},
    {Key::Seven, '&'},
    {Key::Eight, '*'},
    {Key::Nine, '('},
    {Key::Zero, ')'},
    {Key::Period, '>'},
    {Key::SemiColon, ':'},
    {Key::Comma, '<'},
    {Key::RSlash, '?'},
    {Key::LSlash, '|'},
    {Key::QuotationMarkSingle, '"'},
    {Key::Minus, '_'},
    {Key::Equal, '+'},
    {Key::LSquareBracket, '{'},
    {Key::RSquareBracket, '}'},
    {Key::BackTick, '~'},
};
