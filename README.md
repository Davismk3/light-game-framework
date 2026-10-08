# Lightweight Game Framework

A lightweight C++/OpenGL framework for games and other applications. It simplifies initialization, meshing, and rendering while providing built-in screen management, on-screen buttons, and more.

I found myself struggling to scale game/other application projects, and also struggling to start over and relearn libraries when the previous attempt's codebase became too unscalable. This framework was made with the goal of resolving both of these issues. Care was taken to make this both scalable and easy to use.

As the developer, you should build your project in `app/`, and leave `engine/` largely untouched. 

`README.md` was polished by AI. 

## Examples

The following screenshots are taken from *Voxelverse*, a project of mine, which uses the same source code in `engine/` for its core.

Planet, moon, moon, and star:

<img src="assets/cinematic_3.png" alt="1" width="800" height="500">
Procedural rivers:

<img src="assets/cinematic_2.png" alt="2" width="800" height="500">
Procedural mountains:

<img src="assets/cinematic_1.png" alt="3" width="800" height="500">
Small moon's surface:

<img src="assets/cinematic_5.png" alt="4" width="800" height="500">

## Requirements

- GLFW
- GLAD
- stb_image
- stb_image_write

## How To Use

### Setup

Install GLFW (on macOS: `brew install glfw`), then place GLAD and stb in a `vendor/` folder at the project root:

```text
vendor/
├── glad/
│   ├── include/glad/gl.h
│   └── src/gl.c
└── stb/
    ├── stb_image.h
    └── stb_image_write.h
```

### Build and Run

From the project root (macOS):

```sh
g++ -std=c++20 \
  app/*.cpp app/app_controls/*.cpp app/app_screen/*.cpp \
  engine/engine_platform/*.cpp engine/engine_render/*.cpp \
  engine/engine_object/*.cpp engine/engine_utility/*.cpp \
  vendor/glad/src/gl.c \
  -Ivendor/glad/include -Ivendor/stb \
  -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw \
  -framework Cocoa -framework OpenGL -framework IOKit \
  -o game

./game
```

Run `./game` from the project root. Shaders and the font atlas are loaded by relative path (`engine/engine_assets/...`).

### Configuration

Window name, initial size, and icon path are set in `engine/engine_config.hpp`.

### The Application Loop

`app/app.cpp` owns the main loop: `initialize()` → (`processInput()` → `update()` → `render()`) every frame → `shutdown()`. Each phase has a marked section for your own logic:

```cpp
// Place Update Logic Here:

// -
```

Most logic belongs in a screen rather than directly in `app.cpp`.

### Screens

A screen is a self-contained state of the application (title screen, game, settings, etc.). Each screen implements the `Screen` interface in `app/app_screen/screen.hpp`: `initialize`, `processInput`, `update`, `render`, and `shutdown`. `ScreenManager` owns the active screen, and when `current_screen` changes it shuts down the old screen and initializes the new one.

To add a screen:

1. Add a value to `ScreenType` in `screen.hpp`.
2. Create `screen_<name>.hpp/.cpp` with a class deriving from `Screen` (copy `screen_title` as a starting point).
3. Add a `case` for it in `ScreenManager::changeScreen()` in `screen.cpp`.
4. Add a flag to `ControlsState` (e.g. `title_to_settings`), set it from the screen's `processInput`, and switch to the screen in `userControls()` in `app/app_controls/controls.cpp`. Reset the flag in the screen's `shutdown`.

### Controls

`app/app_controls/` translates raw input into app actions. `userControls()` handles player-facing actions, and `developerControls()` holds debug shortcuts (by default, `A`/`S` jump between screens). `ControlsState` is the shared set of flags that screens and controls read and write.

### Menus

`Menu` (`engine/engine_object/menu.hpp`) holds and draws on-screen UI: buttons, textured buttons, text, text boxes, and panels. Positions are in screen coordinates from `-1` to `1`, with `(0, 0)` at the center.

```cpp
// initialize()
ButtonConfig style;
style.width = 0.3f;
style.height = 0.04f;
style.idle_color = {1.0f, 1.0f, 1.0f};
style.hover_color = {0.85f, 0.85f, 0.85f};
style.held_color = {0.7f, 1.0f, 0.7f};
m_menu.addTexturedButton(TexturedButton({0.0f, 0.0f}, style, TexturedButtonType::Large, 1.0f));

TextConfig text_style;
text_style.size = 0.1f;
text_style.color = {1.0f, 1.0f, 1.0f};
text_style.positioning = TextPositioning::Centered;
m_menu.addText(Text({0.0f, 0.8f}, text_style, "Screen 1"));

m_menu.initialize();
m_menu.buildMesh();

// processInput()
m_menu.processInput(input);
if (m_menu.m_texture_buttons[0].m_state == ButtonState::Hover && input.inputMouseReleased(MouseButton::left)) {
    // button clicked
}

// render()
m_menu.draw();
```

UI is drawn with depth testing off: `app.cpp` calls `beginOverlayPass()` before rendering the screen. If you draw 3D geometry, call `beginOpaquePass()` (or `beginTranslucentPass()`) first, then `beginOverlayPass()` before drawing menus.

## Directory Architecture

```text
.
├── app/                              # your project lives here
│   ├── main.cpp                      # entry point, runs Application
│   ├── app.(hpp/cpp)                 # Application: window, input, timer, main loop
│   ├── app_config.hpp                # app-level settings
│   ├── app_controls/
│   │   ├── controls.(hpp/cpp)        # developer and user controls
│   │   └── controls_state.hpp        # ControlsState: shared action flags
│   └── app_screen/
│       ├── screen.(hpp/cpp)          # Screen interface and ScreenManager
│       ├── screen_title.(hpp/cpp)    # example screen 1
│       └── screen_simulation.(hpp/cpp) # example screen 2
│
└── engine/                           # reusable engine, usually left untouched
    ├── engine_config.hpp             # window name, size, icon, font atlas layout
    ├── engine_assets/
    │   ├── basic.(vert/frag)         # default shaders
    │   └── font.png                  # font and UI texture atlas
    ├── engine_platform/
    │   ├── window.(hpp/cpp)          # Window: GLFW window and OpenGL context
    │   └── input.(hpp/cpp)           # Input: keyboard and mouse state
    ├── engine_render/
    │   ├── context.(hpp/cpp)         # GraphicsContext: frame and render-pass state
    │   ├── camera.(hpp/cpp)          # Camera: view and projection matrices
    │   ├── primitive.(hpp/cpp)       # Vertex, Primitive: CPU-side geometry
    │   ├── mesh.(hpp/cpp)            # Mesh: GPU buffers
    │   ├── shader.(hpp/cpp)          # Shader: compile, use, set uniforms
    │   ├── texture.(hpp/cpp)         # Texture: load and bind images
    │   └── draw.(hpp/cpp)            # draw calls and clear
    ├── engine_object/
    │   ├── menu.(hpp/cpp)            # Menu: holds and draws UI objects
    │   ├── button.(hpp/cpp)          # Button, TexturedButton
    │   ├── text.(hpp/cpp)            # Text, TextBox
    │   └── panel.(hpp/cpp)           # Panel: 9-slice UI backdrop
    └── engine_utility/
        ├── math.hpp                  # general math helpers
        ├── vector.hpp                # Vector2/3/4
        ├── matrix.hpp                # Matrix4: transforms, perspective, orthographic
        ├── quaternion.(hpp/cpp)      # Quaternion rotations
        ├── geometry.(hpp/cpp)        # points, segments, planes, polygons, polyhedra
        ├── random.(hpp/cpp)          # random numbers, hashing, value/Brownian noise
        ├── time.(hpp/cpp)            # Timer: delta and elapsed time
        ├── data.hpp                  # binary read/write helpers for save data
        └── png.(hpp/cpp)             # savePNG
```

