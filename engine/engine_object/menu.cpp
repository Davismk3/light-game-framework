#include "menu.hpp"

// Initialize: Loads the menu's shader and the font atlas. p_texture_: the texture used by texture quads, or nullptr if there are none. Now there is the ability to define the shader rather than it be fixed to the engine's default basic shader.
void Menu::initialize(Texture* p_texture_, std::string vertex_path, std::string fragment_path) {
    if (vertex_path == "NONE" || fragment_path == "NONE") m_shader.shaderInitialize("engine/engine_assets/shaders/basic.vert", "engine/engine_assets/shaders/basic.frag");
    else m_shader.shaderInitialize(vertex_path, fragment_path);
    m_shader.shaderUse();
    m_font_texture.textureLoad("engine/engine_assets/textures/ui_atlas.png");

    p_texture = p_texture_;
}

// Build Mesh: Rebuilds the mesh from every object and records each object's index stride. Called by draw when the mesh is dirty.
void Menu::buildMesh() {
    m_mesh.meshClear();
    for (std::vector<IndexStride>& index_strides : m_index_strides) index_strides.clear();

    // Mesh Buttons
    for (Button& button : m_buttons) {
        Primitive primitive = button.buildPrimitives();
        IndexStride button_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(primitive.indices.size())};

        m_mesh.meshAppendPrimitives(primitive);
        m_index_strides[0].push_back(button_stride);
    }

    // Mesh Text
    for (Text& text : m_texts) {
        Primitive primitive = text.buildPrimitives();
        IndexStride text_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(primitive.indices.size())};

        m_mesh.meshAppendPrimitives(primitive);
        m_index_strides[1].push_back(text_stride);
    }

    // Mesh TextBoxes
    for (TextBox& textbox : m_textboxes) {

        // Mesh Box
        Primitive box_primitive = textbox.buildBoxPrimitives();
        IndexStride box_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(box_primitive.indices.size())};
        // -
        m_mesh.meshAppendPrimitives(box_primitive);
        m_index_strides[2].push_back(box_stride);

        // Mesh Text
        Primitive text_primitive = textbox.buildTextPrimitives();
        IndexStride textbox_text_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(text_primitive.indices.size())};
        // - 
        m_mesh.meshAppendPrimitives(text_primitive);
        m_index_strides[3].push_back(textbox_text_stride);

        // Mesh Cursor
        Primitive cursor_primitive = textbox.buildCursorPrimitives();
        IndexStride cursor_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(cursor_primitive.indices.size())};
        // -
        m_mesh.meshAppendPrimitives(cursor_primitive);
        m_index_strides[4].push_back(cursor_stride);
    }

    // Mesh Basic Quads
    for (MenuQuad& menu_quad : m_basic_quads) {
        Primitive basic_quad_primitive = menu_quad.quad_primitive;
        IndexStride basic_quad_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(basic_quad_primitive.indices.size())};
        
        m_mesh.meshAppendPrimitives(basic_quad_primitive);
        m_index_strides[5].push_back(basic_quad_stride);
    }

    // Mesh Texture Quads
    for (MenuQuad& menu_quad : m_texture_quads) {
        Primitive texture_quad_primitive = menu_quad.quad_primitive;
        IndexStride texture_quad_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(texture_quad_primitive.indices.size())};
        
        m_mesh.meshAppendPrimitives(texture_quad_primitive);
        m_index_strides[6].push_back(texture_quad_stride);
    }

    // Mesh Textured Buttons
    for (TexturedButton& texture_button : m_texture_buttons) {
        Primitive texture_button_primitive = texture_button.buildPrimitives();
        IndexStride button_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(texture_button_primitive.indices.size())};

        m_mesh.meshAppendPrimitives(texture_button_primitive);
        m_index_strides[7].push_back(button_stride);
    }
    
    // Mesh Panels
    for (Panel& panel : m_panels) {
        Primitive panel_primitive = panel.buildPrimitives();
        IndexStride panel_stride = {.first_index = static_cast<unsigned int>(m_mesh.m_indices.size()), .index_count = static_cast<unsigned int>(panel_primitive.indices.size())};
    
        m_mesh.meshAppendPrimitives(panel_primitive);
        m_index_strides[8].push_back(panel_stride);
    }

    m_mesh.meshUpload();
    m_mesh_dirty = false;
}

// Process Input: Updates the buttons and textboxes from the user's input, and marks the mesh dirty if a textbox changed.
void Menu::processInput(const Input& input) {
    m_mouse_u = input.inputMouseWindowU() * m_aspect;
    m_mouse_v = input.inputMouseWindowV();

    // Process Buttons
    for (std::size_t i = 0; i < m_buttons.size(); ++i) {
        Button& button = m_buttons[i];
        button.processInput(input, m_aspect);
    }

    // Process Textured Buttons 
    for (std::size_t i = 0; i < m_texture_buttons.size(); ++i) {
        TexturedButton& texture_button = m_texture_buttons[i];
        texture_button.processInput(input, m_aspect);
    }

    // Process Textboxes
    for (TextBox& textbox : m_textboxes) {
        textbox.processInput(input, m_aspect);
        if (textbox.m_text_changed) {
            m_mesh_dirty = true;
        }
        if (textbox.m_state_just_changed) m_mesh_dirty = true;
    }
}

// Update: Toggles the textbox cursor every m_cursor_flash_time seconds. delta_time >= 0, in seconds.
void Menu::update(float delta_time) {
    m_time += delta_time;
    if (m_time >= m_cursor_flash_time) {
        m_time = 0.0f;
        m_mesh_dirty = true;
        m_cursor_toggle_bool = !m_cursor_toggle_bool;
    }
}

// Draw: Rebuilds the mesh if dirty, then draws panels, basic quads, texture quads, textured buttons, buttons, text, and textboxes, in that order.
void Menu::draw() {
    if (m_mesh_dirty) buildMesh();
    m_shader.shaderUse();

    // Bind VAO
    glBindVertexArray(m_mesh.m_VAO);

    // Draw Panels
    for (int i = 0; i < m_panels.size(); i++) {
        Panel& panel = m_panels[i];
        IndexStride& panel_stride = m_index_strides[8][i];

        m_font_texture.textureBind();

        m_shader.shaderSetBool("use_texture", true);
        m_shader.shaderSetFloat("u_opacity", panel.m_panel_config.opacity);
        m_shader.shaderSetVec3("u_tint", panel.m_panel_config.tint.x, panel.m_panel_config.tint.y, panel.m_panel_config.tint.z);
        if (panel.m_panel_config.follows_cursor) m_shader.shaderSetVec3("a_shift", m_mouse_u, m_mouse_v, 0.0f);
        else m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);

        drawMeshRangeExposed(m_mesh, m_shader, panel_stride.first_index, panel_stride.index_count);
    }
    m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);

    // Draw Basic Quads
    for (int i = 0; i < m_basic_quads.size(); i++) {
        MenuQuad& menu_basic_quad = m_basic_quads[i];
        IndexStride& basic_quad_stride = m_index_strides[5][i];

        m_shader.shaderSetBool("use_texture", false);
        m_shader.shaderSetFloat("u_opacity", menu_basic_quad.opacity);
        m_shader.shaderSetVec3("u_tint", menu_basic_quad.tint.x, menu_basic_quad.tint.y, menu_basic_quad.tint.z);
        if (m_basic_quads[i].follows_cursor) m_shader.shaderSetVec3("a_shift", m_mouse_u, m_mouse_v, 0.0f);
        else m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);
        
        drawMeshRangeExposed(m_mesh, m_shader, basic_quad_stride.first_index, basic_quad_stride.index_count);
    }
    m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);

    // Draw Texture Quads
    if (p_texture != nullptr) {
        m_shader.shaderSetBool("use_texture", true);
        p_texture->textureBind();

        for (int i = 0; i < m_texture_quads.size(); i++) {
            MenuQuad& menu_texture_quad = m_texture_quads[i];
            IndexStride& texture_quad_stride = m_index_strides[6][i];

            m_shader.shaderSetFloat("u_opacity", menu_texture_quad.opacity);
            m_shader.shaderSetVec3("u_tint", menu_texture_quad.tint.x, menu_texture_quad.tint.y, menu_texture_quad.tint.z);

            drawMeshRangeExposed(m_mesh, m_shader, texture_quad_stride.first_index, texture_quad_stride.index_count);
        }
    }

    // Draw Textured Buttons
    for (int i = 0; i < m_texture_buttons.size(); i++) {
        const TexturedButton& texture_button = m_texture_buttons[i];
        const ButtonConfig& config = texture_button.m_config;
        IndexStride& button_stride = m_index_strides[7][i];

        Vector3F color = config.idle_color;
        if (texture_button.m_state == ButtonState::Held) color = config.held_color;
        else if (texture_button.m_state == ButtonState::Hover) color = config.hover_color;

        m_font_texture.textureBind();

        m_shader.shaderSetVec3("u_tint", color.x, color.y, color.z);
        m_shader.shaderSetFloat("u_opacity", config.opacity);
        m_shader.shaderSetBool("use_texture", true);

        drawMeshRangeExposed(m_mesh, m_shader, button_stride.first_index, button_stride.index_count); 
    }

    // Draw Buttons
    for (int i = 0; i < m_buttons.size(); i++) {
        const Button& button = m_buttons[i];
        const ButtonConfig& config = button.m_config;
        IndexStride& button_stride = m_index_strides[0][i];

        Vector3F color = config.idle_color;
        if (button.m_state == ButtonState::Held) color = config.held_color;
        else if (button.m_state == ButtonState::Hover) color = config.hover_color;

        m_shader.shaderSetVec3("u_tint", color.x, color.y, color.z);
        m_shader.shaderSetFloat("u_opacity", config.opacity);
        m_shader.shaderSetBool("use_texture", false);

        drawMeshRangeExposed(m_mesh, m_shader, button_stride.first_index, button_stride.index_count); 
    }

    // Draw Text
    m_font_texture.textureBind();
    for (int i = 0; i < m_texts.size(); i++) {
        Text& text = m_texts[i];
        TextConfig& config = text.m_config;
        Vector3F color = config.color;
        IndexStride& text_stride = m_index_strides[1][i];

        if (text.m_config.has_background) {
            m_shader.shaderSetBool("use_texture", false);
            Vector3F background_color = config.background_color;
            // -
            m_shader.shaderSetVec3("u_tint", background_color.x, background_color.y, background_color.z);
            m_shader.shaderSetFloat("u_opacity", config.background_opacity);
            if (text.follows_cursor) m_shader.shaderSetVec3("a_shift", m_mouse_u, m_mouse_v, 0.0f);
            else m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);
            // - 
            drawMeshRangeExposed(m_mesh, m_shader, text_stride.first_index, 6);
        }
        m_shader.shaderSetBool("use_texture", true);
        m_shader.shaderSetFloat("u_opacity", config.opacity);

        // Background Text
        if (text.m_config.has_shadow) {
            Vector3F shadow_color = config.shadow_color;
            // -
            m_shader.shaderSetVec3("u_tint", shadow_color.x, shadow_color.y, shadow_color.z);
            if (text.follows_cursor) m_shader.shaderSetVec3("a_shift", m_mouse_u + config.size / static_cast<float>(GLYPH_HEIGHT), m_mouse_v - config.size / static_cast<float>(GLYPH_HEIGHT), 0.0f);
            else m_shader.shaderSetVec3("a_shift", config.size / static_cast<float>(GLYPH_HEIGHT), -config.size / static_cast<float>(GLYPH_HEIGHT), 0.0f);
            // - 
            drawMeshRangeExposed(m_mesh, m_shader, text_stride.first_index + 6, text_stride.index_count - 6);
        }

        // Foreground Text
        m_shader.shaderSetVec3("u_tint", color.x, color.y, color.z);
        if (text.follows_cursor) m_shader.shaderSetVec3("a_shift", m_mouse_u, m_mouse_v, 0.0f);
        else m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);
        // - 
        drawMeshRangeExposed(m_mesh, m_shader, text_stride.first_index + 6, text_stride.index_count - 6);
    }
    m_shader.shaderSetVec3("a_shift", 0.0f, 0.0f, 0.0f);

    // Draw TextBoxes
    for (int i = 0; i < m_textboxes.size(); i++) {
        TextBox& textbox = m_textboxes[i];
        TextBoxConfig& config = textbox.m_config;
        Vector3F text_color = textbox.m_combined_current_text.m_config.color;
        IndexStride& box_stride = m_index_strides[2][i];
        IndexStride& text_stride = m_index_strides[3][i];
        IndexStride& cursor_stride = m_index_strides[4][i];

        Vector3F box_color = config.idle_color;
        if (textbox.m_state == TextBoxState::Hover) box_color = config.hover_color;
        else if (textbox.m_state == TextBoxState::Active) box_color = config.hover_color;
        
        // Draw Box
        m_shader.shaderSetBool("use_texture", false);
        m_shader.shaderSetFloat("u_opacity", config.opacity);
        m_shader.shaderSetVec3("u_tint", box_color.x, box_color.y, box_color.z);

        drawMeshRangeExposed(m_mesh, m_shader, box_stride.first_index, box_stride.index_count);

        // Draw Text
        m_shader.shaderSetBool("use_texture", true);
        m_shader.shaderSetVec3("u_tint", text_color.x, text_color.y, text_color.z);
        m_shader.shaderSetFloat("u_opacity", textbox.m_combined_current_text.m_config.opacity);
        drawMeshRangeExposed(m_mesh, m_shader, text_stride.first_index + 6, text_stride.index_count - 6);

        // Draw Cursor
        if (textbox.m_state == TextBoxState::Active && m_cursor_toggle_bool) drawMeshRangeExposed(m_mesh, m_shader, cursor_stride.first_index, cursor_stride.index_count);
    
    }

    // Unbind VAO
    glBindVertexArray(0);
}

// Shutdown: Currently empty.
void Menu::shutdown() {

}

// Add Button: Adds a button and marks the mesh dirty.
void Menu::addButton(Button button) {
    m_buttons.push_back(button);
    m_mesh_dirty = true;
}

// Add Textured Button: Adds a textured button and marks the mesh dirty.
void Menu::addTexturedButton(TexturedButton button) {
    m_texture_buttons.push_back(button);
    m_mesh_dirty = true;
}

// Add Text: Adds text and marks the mesh dirty.
void Menu::addText(Text text) {
    m_texts.push_back(text);
    m_mesh_dirty = true;
}

// Add Text Box: Adds a textbox and marks the mesh dirty. Call initializeDefault on the textbox first.
void Menu::addTextBox(TextBox textbox) {
    m_textboxes.push_back(textbox);
    m_mesh_dirty = true;
}

// Add Basic Quad: Adds an untextured quad and marks the mesh dirty.
void Menu::addBasicQuad(MenuQuad menu_quad) {
    m_basic_quads.push_back(menu_quad);
    m_mesh_dirty = true;
}

// Add Texture Quad: Adds a quad textured with p_texture and marks the mesh dirty.
void Menu::addTextureQuad(MenuQuad menu_quad) {
    m_texture_quads.push_back(menu_quad);
    m_mesh_dirty = true;
}

// Add Panel: Adds a panel and marks the mesh dirty.
void Menu::addPanel(Panel panel) {
    m_panels.push_back(panel);
    m_mesh_dirty = true;
}

// Resize: projection: the menu's projection matrix for the new window size. aspect > 0, the window's width / height.
void Menu::resize(Matrix4F projection, float aspect) {
    m_shader.shaderUse();
    m_shader.shaderSetMat4("u_projection", projection); 
    float aspect_ratio = aspect / m_aspect;
    m_aspect = aspect;
    m_mesh_dirty = true;
}
