#include "context.hpp"

// Graphics Context Initialize: Enables depth testing and alpha blending. Call once after the OpenGL context is created.
void graphicsContextInitialize() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Begin Frame: Clears the color and depth buffers to black.
void graphicsContextBeginFrame() {
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Begin Opaque Pass: Depth test and depth writes on, blending off, back faces culled.
void graphicsContextBeginOpaquePass() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

// Begin Translucent Pass: Depth test on, depth writes off, blending on, no culling.
void graphicsContextBeginTranslucentPass() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDepthMask(GL_FALSE);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glDisable(GL_CULL_FACE);
}

// Begin Translucent Pass V2: Depth test and depth writes on, blending on, back faces culled.
void graphicsContextBeginTranslucentPassV2() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDepthMask(GL_TRUE);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

// Begin Translucent Two Pass A: Writes depth only, no color. Follow with beginTranslucentTwoPassB.
void graphicsContextBeginTranslucentTwoPassA() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

// Begin Translucent Two Pass B: Draws color only where depth equals pass A's, with blending on and no culling.
void graphicsContextBeginTranslucentTwoPassB() {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_EQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE); // if water should be two-sided
}

// Begin Overlay Pass: Depth test and depth writes off, blending on, no culling. Used for menus and the HUD.
void graphicsContextBeginOverlayPass() {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
}

// Begin Reverse Projection Pass: Depth test passes for greater depth, and the clear depth is set to 0. Currently unused.
void graphicsContextBeginReverseProjectionPass() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glClearDepth(0.0f);
}

// End Frame: Currently empty.
void graphicsContextEndFrame() {
    // possibly debug overlays
}

// Resize: width, height, in pixels. Ignored if either is <= 0.
void graphicsContextResize(int width, int height) {
    if (width <= 0 || height <= 0) return;
    glViewport(0, 0, width, height);
}
