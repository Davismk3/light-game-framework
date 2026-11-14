#include "panel.hpp"

// Build Primitives: Returns nine quads from the font atlas: the center, four sides, and four corners. The border's thickness scales with pixel_size.
Primitive Panel::buildPrimitives() {
    Primitive panel_primitive;
    // 9 quads total. 
    // 4 corners, 4 sides, 1 middle
    float size = m_panel_config.pixel_size;

    // Center (screen coords)
    const float center_l = m_panel_config.center_position.u - m_panel_config.width;
    const float center_r = m_panel_config.center_position.u + m_panel_config.width;
    const float center_b = m_panel_config.center_position.v - m_panel_config.height;
    const float center_t = m_panel_config.center_position.v + m_panel_config.height;
    // - (texture coords)
    float center_u0 = 111.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float center_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float center_u1 = 121.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float center_v1 = 1.0f - 82.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {center_r, center_t, 0.0f, center_u1, center_v0},
        {center_r, center_b, 0.0f, center_u1, center_v1},
        {center_l, center_b, 0.0f, center_u0, center_v1},
        {center_l, center_t, 0.0f, center_u0, center_v0}
    ));

    // Right Side (screen coords)
    float right_side_center_position_u = m_panel_config.center_position.u + m_panel_config.width;  // the side is skewed to the quad's side. not sure if this is the right factor

    const float right_side_l = right_side_center_position_u ;  // the side is more narrow than the main quad by a factor of 5/12;
    const float right_side_r = right_side_center_position_u + m_panel_config.width * size * m_panel_config.height / 12.0f;  
    const float right_side_b = m_panel_config.center_position.v - m_panel_config.height;
    const float right_side_t = m_panel_config.center_position.v + m_panel_config.height;
    // - (texture coords)
    float right_side_u0 = 92.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float right_side_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float right_side_u1 = 96.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float right_side_v1 = 1.0f - 82.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {right_side_r, right_side_t, 0.0f, right_side_u1, right_side_v0},
        {right_side_r, right_side_b, 0.0f, right_side_u1, right_side_v1},
        {right_side_l, right_side_b, 0.0f, right_side_u0, right_side_v1},
        {right_side_l, right_side_t, 0.0f, right_side_u0, right_side_v0}
    ));

    // Left Side (screen coords) 
    float left_side_center_position_u = m_panel_config.center_position.u - m_panel_config.width;  // the side is skewed to the quad's side. not sure if this is the right factor

    const float left_side_l = left_side_center_position_u - m_panel_config.width * size * m_panel_config.height / 12.0f;  // the side is more narrow than the main quad by a factor of 5/12;
    const float left_side_r = left_side_center_position_u;  
    const float left_side_b = m_panel_config.center_position.v - m_panel_config.height;
    const float left_side_t = m_panel_config.center_position.v + m_panel_config.height;
    // - (texture coords)
    float left_side_u0 = 85.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float left_side_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float left_side_u1 = 90.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float left_side_v1 = 1.0f - 82.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {left_side_r, left_side_t, 0.0f, left_side_u1, left_side_v0},
        {left_side_r, left_side_b, 0.0f, left_side_u1, left_side_v1},
        {left_side_l, left_side_b, 0.0f, left_side_u0, left_side_v1},
        {left_side_l, left_side_t, 0.0f, left_side_u0, left_side_v0}
    ));

    // Top Side (screen coords)
    float top_side_center_position_v = m_panel_config.center_position.v + m_panel_config.height;  // the side is skewed to the quad's side. not sure if this is the right factor

    const float top_side_l = m_panel_config.center_position.u - m_panel_config.width; 
    const float top_side_r = m_panel_config.center_position.u + m_panel_config.width;
    const float top_side_b = top_side_center_position_v;
    const float top_side_t = top_side_center_position_v + m_panel_config.height * size * m_panel_config.width / 12.0f;;
    // - (texture coords)
    float top_side_u0 = 97.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float top_side_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float top_side_u1 = 108.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float top_side_v1 = 1.0f - 75.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {top_side_r, top_side_t, 0.0f, top_side_u1, top_side_v0},
        {top_side_r, top_side_b, 0.0f, top_side_u1, top_side_v1},
        {top_side_l, top_side_b, 0.0f, top_side_u0, top_side_v1},
        {top_side_l, top_side_t, 0.0f, top_side_u0, top_side_v0}
    ));

    // Bottom Side (screen coords)
    float bottom_side_center_position_v = m_panel_config.center_position.v - m_panel_config.height;  // the side is skewed to the quad's side. not sure if this is the right factor

    const float bottom_side_l = m_panel_config.center_position.u - m_panel_config.width; 
    const float bottom_side_r = m_panel_config.center_position.u + m_panel_config.width;
    const float bottom_side_b = bottom_side_center_position_v - m_panel_config.height * size * m_panel_config.width / 12.0f;
    const float bottom_side_t = bottom_side_center_position_v;
    // - (texture coords)
    float bottom_side_u0 = 97.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float bottom_side_v0 = 1.0f - 78.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float bottom_side_u1 = 108.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float bottom_side_v1 = 1.0f - 82.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {bottom_side_r, bottom_side_t, 0.0f, bottom_side_u1, bottom_side_v0},
        {bottom_side_r, bottom_side_b, 0.0f, bottom_side_u1, bottom_side_v1},
        {bottom_side_l, bottom_side_b, 0.0f, bottom_side_u0, bottom_side_v1},
        {bottom_side_l, bottom_side_t, 0.0f, bottom_side_u0, bottom_side_v0}
    ));

    // Top Right Corner (screen coords)
    float tr_corner_center_position_u = m_panel_config.center_position.u + m_panel_config.width;  // the side is skewed to the quad's side. not sure if this is the right factor
    float tr_corner_center_position_v = m_panel_config.center_position.v + m_panel_config.height;  // the side is skewed to the quad's side. not sure if this is the right factor

    const float tr_corner_l = tr_corner_center_position_u ; 
    const float tr_corner_r = tr_corner_center_position_u + m_panel_config.width * size * m_panel_config.height / 12.0f;;
    const float tr_corner_b = tr_corner_center_position_v;
    const float tr_corner_t = tr_corner_center_position_v + m_panel_config.height * size * m_panel_config.width / 12.0f;
    // - (texture coords)
    float tr_corner_u0 = 80.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float tr_corner_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float tr_corner_u1 = 84.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float tr_corner_v1 = 1.0f - 75.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {tr_corner_r, tr_corner_t, 0.0f, tr_corner_u1, tr_corner_v0},
        {tr_corner_r, tr_corner_b, 0.0f, tr_corner_u1, tr_corner_v1},
        {tr_corner_l, tr_corner_b, 0.0f, tr_corner_u0, tr_corner_v1},
        {tr_corner_l, tr_corner_t, 0.0f, tr_corner_u0, tr_corner_v0}
    ));

    // Top Left Corner (screen coords)
    float tl_corner_center_position_u = m_panel_config.center_position.u - m_panel_config.width;
    float tl_corner_center_position_v = m_panel_config.center_position.v + m_panel_config.height;

    const float tl_corner_l = tl_corner_center_position_u - m_panel_config.width * size * m_panel_config.height / 12.0f;
    const float tl_corner_r = tl_corner_center_position_u;
    const float tl_corner_b = tl_corner_center_position_v;
    const float tl_corner_t = tl_corner_center_position_v + m_panel_config.height * size * m_panel_config.width / 12.0f;
    // - (texture coords)
    float tl_corner_u0 = 72.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float tl_corner_v0 = 1.0f - 71.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float tl_corner_u1 = 76.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float tl_corner_v1 = 1.0f - 75.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {tl_corner_r, tl_corner_t, 0.0f, tl_corner_u1, tl_corner_v0},
        {tl_corner_r, tl_corner_b, 0.0f, tl_corner_u1, tl_corner_v1},
        {tl_corner_l, tl_corner_b, 0.0f, tl_corner_u0, tl_corner_v1},
        {tl_corner_l, tl_corner_t, 0.0f, tl_corner_u0, tl_corner_v0}
    ));

    // Bottom Right Corner (screen coords)
    float br_corner_center_position_u = m_panel_config.center_position.u + m_panel_config.width;
    float br_corner_center_position_v = m_panel_config.center_position.v - m_panel_config.height;

    const float br_corner_l = br_corner_center_position_u;
    const float br_corner_r = br_corner_center_position_u + m_panel_config.width * size * m_panel_config.height / 12.0f;
    const float br_corner_b = br_corner_center_position_v - m_panel_config.height * size * m_panel_config.width / 12.0f;
    const float br_corner_t = br_corner_center_position_v;
    // - (texture coords)
    float br_corner_u0 = 80.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float br_corner_v0 = 1.0f - 79.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float br_corner_u1 = 84.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float br_corner_v1 = 1.0f - 83.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {br_corner_r, br_corner_t, 0.0f, br_corner_u1, br_corner_v0},
        {br_corner_r, br_corner_b, 0.0f, br_corner_u1, br_corner_v1},
        {br_corner_l, br_corner_b, 0.0f, br_corner_u0, br_corner_v1},
        {br_corner_l, br_corner_t, 0.0f, br_corner_u0, br_corner_v0}
    ));

    // Bottom Left Corner (screen coords)
    float bl_corner_center_position_u = m_panel_config.center_position.u - m_panel_config.width;
    float bl_corner_center_position_v = m_panel_config.center_position.v - m_panel_config.height;

    const float bl_corner_l = bl_corner_center_position_u - m_panel_config.width * size * m_panel_config.height / 12.0f;
    const float bl_corner_r = bl_corner_center_position_u;
    const float bl_corner_b = bl_corner_center_position_v - m_panel_config.height * size * m_panel_config.width / 12.0f;
    const float bl_corner_t = bl_corner_center_position_v;
    // - (texture coords)
    float bl_corner_u0 = 72.0f / FONT_AXIS_WIDTH;        // upper left u pixel
    float bl_corner_v0 = 1.0f - 79.0f / FONT_AXIS_HEIGHT; // upper left v pixel
    float bl_corner_u1 = 76.0f / FONT_AXIS_WIDTH;        // lower right u pixel
    float bl_corner_v1 = 1.0f - 83.0f / FONT_AXIS_HEIGHT; // lower right v pixel
    // -
    panel_primitive.appendPrimitive(quad(
        {bl_corner_r, bl_corner_t, 0.0f, bl_corner_u1, bl_corner_v0},
        {bl_corner_r, bl_corner_b, 0.0f, bl_corner_u1, bl_corner_v1},
        {bl_corner_l, bl_corner_b, 0.0f, bl_corner_u0, bl_corner_v1},
        {bl_corner_l, bl_corner_t, 0.0f, bl_corner_u0, bl_corner_v0}
    ));

    return panel_primitive;
}
