#pragma once

#include "text.hpp"
#include "panel.hpp"
#include "button.hpp"

#include <cstddef>
#include <optional>

// Menu Quad: A standalone quad drawn by a menu, either untextured or textured.
struct MenuQuad {

    // Attributes
    Primitive quad_primitive;  // usually a single quad, in window UV units
    float opacity = 1.0f;  // ∈ [0.0f, 1.0f]
    Vector3F tint = {1.0f, 1.0f, 1.0f};  // rgb ∈ [0.0f, 1.0f]
    bool follows_cursor = false;  // shifts the quad by the mouse position, basic quads only
};

// Index Stride: The range of a menu's mesh indices that belong to one object.
struct IndexStride {

    // Range
    unsigned int first_index;  // starting index in primitives
    unsigned int index_count;  // how many indices to use
};

// Menu: Stores and renders buttons, text, and other menu related objects.
class Menu {
public:
    ~Menu() = default;
    void initialize(Texture* p_texture_ = nullptr, std::string vertex_path="NONE", std::string fragment_path="NONE");
    void buildMesh();
    void processInput(const Input& input);
    void draw();
    void update(float delta_time);
    void resize(Matrix4F projection, float aspect);
    void shutdown();

    void addButton(Button button);
    void addTexturedButton(TexturedButton button);
    void addText(Text text);
    void addTextBox(TextBox textbox);
    void addBasicQuad(MenuQuad menu_quad);
    void addTextureQuad(MenuQuad menu_quad);
    void addPanel(Panel panel);

    // Objects
    std::vector<Button>         m_buttons;  // untextured, drawn with their state's color
    std::vector<TexturedButton> m_texture_buttons;  // drawn from the font atlas with their state's color
    std::vector<Text>           m_texts;  // drawn with an optional background and shadow
    std::vector<TextBox>        m_textboxes;  // box, text, and cursor are meshed separately
    std::vector<MenuQuad>       m_basic_quads;  // untextured
    std::vector<MenuQuad>       m_texture_quads;  // drawn with p_texture, skipped if it is nullptr
    std::vector<Panel>          m_panels;  // drawn from the font atlas

    // Mesh State
    std::array<std::vector<IndexStride>, 9> m_index_strides;  // one list per object type, indexed in the order buildMesh appends them
    bool m_mesh_dirty = true;  // rebuilds the mesh on the next draw

private:

    // Rendering
    Mesh m_mesh;  // every object's primitives, rebuilt when dirty
    Shader m_shader;  // basic.vert and basic.frag

    // Textures
    Texture* p_texture = nullptr;  // used by texture quads, may be nullptr
    Texture m_font_texture;  // font.png, used by text, panels, and textured buttons

    // Textbox Cursor Flashing
    float m_time = 0.0f;  // seconds since the cursor last toggled
    float m_cursor_flash_time = 0.25f;  // seconds between cursor toggles

    // Window
    float m_aspect = 1.0f;  // window width / height

    // Textbox Cursor Visibility
    bool m_cursor_toggle_bool = false;  // whether the textbox cursor is currently shown

    // Mouse Position
    float m_mouse_u = 0.0f;  // scaled by m_aspect, in window UV units
    float m_mouse_v = 0.0f;  // in window UV units
};
