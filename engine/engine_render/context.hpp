#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void graphicsContextInitialize();
void graphicsContextBeginFrame();
void graphicsContextBeginOpaquePass();
void graphicsContextBeginTranslucentPass();
void graphicsContextBeginTranslucentPassV2();
void graphicsContextBeginTranslucentTwoPassA();
void graphicsContextBeginTranslucentTwoPassB();
void graphicsContextBeginOverlayPass();
void graphicsContextBeginReverseProjectionPass();
void graphicsContextEndFrame();
void graphicsContextResize(int width, int height);
