#include "text.hpp"

namespace {

    // Get Glyph Atlas Texture UVs: Returns the glyph's upper left pixel in the font atlas. glyph_id >= 1, a key of Glyph_To_GlyphId_Map or a special character's ID.
    GlyphAtlasPositionUV getGlyphAtlasTextureUVs(int glyph_id) {
        int index = glyph_id - 1;

        int u = 1 + (glyph_id - 1) % GLYPHS_PER_ROW * (GLYPH_WIDTH + 1);
        int v = 1 + index / GLYPHS_PER_ROW * (GLYPH_HEIGHT + 1);

        return {u, v};
    }

    // Special Chars: Returns the glyph ID for the special character after string[i], or 70 ('/') if there is none. string[i] is the special character's start, and i + 1 < string.size().
    int specialChars(char glyph, std::string& string, int i) {
        // special characters alter the appearance and shift the frame of on-screen text.
        // however, the string itself remains unaffected. the string "/1" is still stored as "/1".
        char next_glyph = string[i + 1];

        // Emoticons
        if (next_glyph == '1') return 96;
        else if (next_glyph == '2') return 97;
        else if (next_glyph == '3') return 98;
        else if (next_glyph == '4') return 99;
        else if (next_glyph == '5') return 100;
        else if (next_glyph == '6') return 101;
        else if (next_glyph == '7') return 102;
        else if (next_glyph == '8') return 103;
        else if (next_glyph == '9') return 104;

        // Mathematics
        else if (next_glyph == '=') return 131;
        else if (next_glyph == '.') return 132;
        else if (next_glyph == '^') return 133;
        else if (next_glyph == '+') return 134;
        else if (next_glyph == 'S') return 135;
        else if (next_glyph == '!') return 136;
        else if (next_glyph == 'E') return 137;
        else if (next_glyph == 'A') return 138;
        else if (next_glyph == '\'')return 139;
        else if (next_glyph == 'p') return 140;
        else if (next_glyph == '~') return 141;
        else if (next_glyph == 'g') return 142;
        else if (next_glyph == 'd') return 143;
        else if (next_glyph == 'o') return 105;
        else if (next_glyph == 'a') return 106;
        else if (next_glyph == '&') return 107;
        else if (next_glyph == 'f') return 157;

        else return 70;
    }

    // Glyph Step: Returns how far to advance after this glyph, in window UV units. width: currently unused. style: only style.size is used. fix_step: true always returns the full 6 pixel step.
    float glyphStep(char glyph, float width, TextConfig style, bool fix_step) {
        auto stepPixel = [](int pixel, float width, TextConfig style) { return pixel * style.size / static_cast<float>(GLYPH_HEIGHT); };
        float step = stepPixel(6, width, style);
        if (fix_step) return step;
        else if (glyph == ' ') step = stepPixel(3, width, style);
        else if (glyph == 'i') step = stepPixel(4, width, style);
        else if (glyph == 'r') step = stepPixel(5, width, style);
        else if (glyph == 'h') step = stepPixel(5, width, style);
        else if (glyph == 'n') step = stepPixel(5, width, style);
        else if (glyph == 'u') step = stepPixel(5, width, style);
        else if (glyph == 'y') step = stepPixel(5, width, style);
        else if (glyph == 'k') step = stepPixel(5, width, style);
        else if (glyph == 'l') step = stepPixel(3, width, style);
        else if (glyph == 't') step = stepPixel(4, width, style);
        else if (glyph == 'j') step = stepPixel(4, width, style);
        else if (glyph == '!') step = stepPixel(2, width, style);
        else if (glyph == ',') step = stepPixel(3, width, style);
        else if (glyph == '.') step = stepPixel(2, width, style);
        else if (glyph == ';') step = stepPixel(3, width, style);
        else if (glyph == ':') step = stepPixel(2, width, style);
        else if (glyph == '<') step = stepPixel(4, width, style);
        else if (glyph == '>') step = stepPixel(4, width, style);
        else if (glyph == '\'')step = stepPixel(3, width, style);
        else if (glyph == 'I') step = stepPixel(5, width, style);
        else if (glyph == 'J') step = stepPixel(5, width, style);
        else if (glyph == '|') step = stepPixel(2, width, style);
        else if (glyph == ')') step = stepPixel(4, width, style);
        else if (glyph == '(') step = stepPixel(4, width, style);
        else if (glyph == ']') step = stepPixel(4, width, style);
        else if (glyph == '[') step = stepPixel(4, width, style);
        else if (glyph == '}') step = stepPixel(4, width, style);
        else if (glyph == '{') step = stepPixel(4, width, style);
        else if (glyph == '*') step = stepPixel(4, width, style);

        return step;
    }
}

// Text: start_position: normalized to ([-1, 1], [-1, 1]) for screen. style: size, colors, background, opacity, and positioning. string: may contain special characters.
Text::Text(Vector2F start_position, TextConfig style, std::string string) {
    m_start_position = start_position;
    m_config = style;
    m_string = string;
}

// Build Primitives: Returns a background quad followed by one quad per glyph. Also sets m_string_length and moves m_end_position forward.
Primitive Text::buildPrimitives() {
    Primitive primitives;
    float width = m_config.size * static_cast<float>(GLYPH_WIDTH) / static_cast<float>(GLYPH_HEIGHT);
    float height = m_config.size;
    float advance = 0.0f;

    Vertex background_bottom_left;
    Vertex background_bottom_right;
    Vertex background_top_right;
    Vertex background_top_left;

    // Primitive For Each Char
    for (int i = 0; i < m_string.size(); i++) {
        bool fix_step = false;
        bool special_exists = false;
        char& glyph = m_string[i];
        int glyph_id = 26;  // placeholder unknown char '?'

        // Get Glyph ID (looks nasty, does work)
        if (m_string[i - 1] == m_special_init) fix_step = true;
        if (glyph == m_special_init && i + 1 < static_cast<int>(m_string.size())) {
            glyph_id = specialChars(glyph, m_string, i);
            if (glyph_id != 70) special_exists = true;
        } else if (Glyph_To_GlyphId_Map.find(glyph) != Glyph_To_GlyphId_Map.end()) {
            glyph_id = Glyph_To_GlyphId_Map.find(glyph)->second; 
            if (m_string[i - 1] == m_special_init && specialChars(glyph, m_string, i - 1) != 70) glyph_id = 74;  // prev = / and curr is special
        }

        // Get UVs
        GlyphAtlasPositionUV glyph_uv = getGlyphAtlasTextureUVs(glyph_id);
        float u0 = static_cast<float>(glyph_uv.u) / FONT_AXIS_WIDTH;
        float v0 = 1.0f - static_cast<float>(glyph_uv.v) / FONT_AXIS_HEIGHT;
        float u1 = static_cast<float>(glyph_uv.u + GLYPH_WIDTH) / FONT_AXIS_WIDTH;
        float v1 = 1.0f - static_cast<float>(glyph_uv.v + GLYPH_HEIGHT) / FONT_AXIS_HEIGHT;

        // Left-To-Right Initialized
        float start_x = m_start_position.u;
        float start_y = m_start_position.v;

        // Centered
        if (m_config.positioning == TextPositioning::Centered) {
            float text_width = 0.0f;
            for (int j = 0; j < m_string.size(); j++) text_width += glyphStep(m_string[j], width, m_config, fix_step);
            start_x -= text_width * 0.5f;
            start_y -= m_config.size * 0.5f;

        // Right-To-Left
        } else if (m_config.positioning == TextPositioning::RightToLeft) {
            float text_width = 0.0f;
            for (int j = 0; j < m_string.size(); j++) text_width += glyphStep(m_string[j], width, m_config, fix_step);
            start_x -= text_width;
        }

        // Append Primitives
        float x0 = start_x + advance;
        float y0 = start_y;
        float x1 = x0 + width;
        float y1 = y0 + height;
        Vertex bottom_left  = {x1, y1, 0.0f, u1, v0};
        Vertex bottom_right = {x1, y0, 0.0f, u1, v1};
        Vertex top_right    = {x0, y0, 0.0f, u0, v1};
        Vertex top_left     = {x0, y1, 0.0f, u0, v0};
        primitives.appendPrimitive(quad(top_right, bottom_right, bottom_left, top_left));

        // Required Handling For Preventing Double Advancement For Special Characters
        if (!special_exists) advance += glyphStep(glyph, width, m_config, fix_step);;

        if (i == 0) {
            background_top_right = top_right;
            background_top_left = top_left;
        } 
        if (i == static_cast<int>(m_string.size()) - 1) {
            background_bottom_right = bottom_right;
            background_bottom_left = bottom_left;
        }
    }
    m_end_position.u += advance;
    m_string_length = advance;
    //std::cout << advance << "\n";

    Primitive background_primitive = quad(
        background_bottom_left,
        background_bottom_right,
        background_top_right,
        background_top_left
    );

    Primitive final_primitive = background_primitive;
    final_primitive.appendPrimitive(primitives);

    return final_primitive;
}

// Initialize Default: Sets up the typed text, empty placeholder text, and cursor from m_position and m_config. Call after setting both.
void TextBox::initializeDefault() {

    // User's Text
    m_textbox_text.m_start_position = {
        m_position.u - 0.95f * m_config.width,
        m_position.v - m_config.height * 0.5f
    };
    m_textbox_text.m_config = {
        .size = m_config.height,
        .color = {1.0f, 1.0f, 1.0f},
        .has_shadow = false,
        .opacity = 1.0f
    };
    m_textbox_text.m_end_position = m_textbox_text.m_start_position;

    // Empty Text
    m_textbox_empty_text.m_start_position = {
        m_position.u - 0.95f * m_config.width,
        m_position.v - m_config.height * 0.5f
    };
    m_textbox_empty_text.m_config = {
        .size = m_config.height,
        .color = {0.5f, 0.5f, 0.5f},
        .has_shadow = false,
        .opacity = 1.0f
    };
    m_textbox_empty_text.m_string = "Type here.";

    // Cursor
    m_textbox_cursor.m_start_position = {
        m_position.u - 0.95f * m_config.width,
        m_position.v - m_config.height * 0.5f
    };
    m_textbox_cursor.m_config = {
        .size = m_config.height,
        .color = {0.5f, 0.5f, 0.5f},
        .has_shadow = false,
        .opacity = 1.0f
    };
    m_textbox_cursor.m_string = "<";
}

// Add Char: char_to_add: any char. Chars missing from Glyph_To_GlyphId_Map draw as a placeholder.
void TextBox::addChar(char char_to_add) {
    m_textbox_text.m_string.push_back(char_to_add);
}

// Subtract Char: Removes the last char, if there is one.
void TextBox::subtractChar() {
    if (m_textbox_text.m_string.empty()) return;
    m_textbox_text.m_string.pop_back();
}

// Is Hover: Is the user's mouse hovering over this textbox? aspect > 0, the window's width / height.
bool TextBox::isHover(const Input& input, float aspect) {
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

// Process Input: Activates the textbox on click, then adds or removes chars while it is active. aspect > 0, the window's width / height.
void TextBox::processInput(const Input& input, float aspect) {
    m_text_changed = false;
    m_state_just_changed = false;

    if (m_combined_current_text.m_string_length > 1.9f * m_config.width - 0.0f) m_string_at_max_length = true;
    else m_string_at_max_length = false;
    //std::cout << m_combined_current_text.m_string_length << " " << m_config.width << "\n";

    if (input.inputMousePressed(MouseButton::left)) {
        if (isHover(input, aspect)) m_is_active = true;
        else m_is_active = false;
        m_state_just_changed = true;
    }

    if (m_is_active) {
        m_state = TextBoxState::Active;

        // Subtract Character
        if (input.inputKeyPressed(Key::Delete)) {
            subtractChar();
            m_text_changed = true;
        }

        for (int i = 0; i < static_cast<int>(Key::COUNT); i++) {
            Key key = static_cast<Key>(i);

            // check which/if key pressed
            if (input.inputKeyPressed(static_cast<Key>(key)) && !m_string_at_max_length) {

                // Early Exit Unknown Key
                if (Key_To_Char_Map.find(key) == Key_To_Char_Map.end()) return;
                
                // Shifted Keys
                if (input.inputKeyDown(Key::RShift) || input.inputKeyDown(Key::LShift)) {
                    char char_to_add = ShiftedKey_To_Char_Map.at(key);
                    addChar(char_to_add);
                    m_text_changed = true;
                
                // Regular Keys
                } else {
                    char char_to_add = Key_To_Char_Map.at(key);
                    addChar(char_to_add);
                    m_text_changed = true;
                }
            }
        }
    } 
    else if (isHover(input, aspect)) m_state = TextBoxState::Hover;
    else m_state = TextBoxState::Idle;

}

// Build Box Primitives: Returns a single untextured quad covering this textbox.
Primitive TextBox::buildBoxPrimitives() {
    const float left = m_position.u - m_config.width;
    const float right = m_position.u + m_config.width;
    const float bottom = m_position.v - m_config.height;
    const float top = m_position.v + m_config.height;

    return quad(
        {right, top, 0.0f, 1.0f, 1.0f},
        {right, bottom, 0.0f, 1.0f, 0.0f},
        {left, bottom, 0.0f, 0.0f, 0.0f},
        {left, top, 0.0f, 0.0f, 1.0f}
    );
}

// Build Text Primitives: Returns the typed text, or the placeholder while empty and inactive. Stores the chosen text in m_combined_current_text.
Primitive TextBox::buildTextPrimitives() {
    if (m_textbox_text.m_string.empty() && m_state != TextBoxState::Active) m_combined_current_text = m_textbox_empty_text;
    else m_combined_current_text = m_textbox_text;
    return m_combined_current_text.buildPrimitives();
}

// Build Cursor Primitives: Returns the cursor at the end of the current text. Call after buildTextPrimitives.
Primitive TextBox::buildCursorPrimitives() {
    if (m_textbox_text.m_string.empty()) m_textbox_cursor.m_start_position = m_textbox_text.m_start_position;
    else m_textbox_cursor.m_start_position = m_combined_current_text.m_end_position;
    return m_textbox_cursor.buildPrimitives();
}
