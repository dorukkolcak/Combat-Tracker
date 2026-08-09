# CombatTracker

CombatTracker is a visualization and tracking tool for D&D 5e encounters, with some experimental and homebrew features.

## Motivation

Frankly there are a LOT of alternatives to a tool like this. The main reasons I wanted to write one anyway are as
follows:

1. I have some homebrew rules/mechanics I use in my games that change depending on the campaign. None of the
   existing tools can support tracking my homebrew mechanics. Having my own tool will allow me to possibly support these mechanics.
2. It is much more convenient to use a tool I designed myself than to use an external one. Many TTRPG tools also
   have quite steep learning curves, and frankly speaking, I am not interested in spending the time to learn
   one of them. But I am somehow interested in creating a tool from scratch...

## Features

- Encounter creation, management and saving.
- Monster and player character creation and saving.
- Starting and running encounters, tracking them on a turn-by-turn basis.
- Interactive encounter battle map visualization, currently planned to be 2D.
- Viewing completed encounters.
- Other custom and homebrew mechanic support.

## Planned Features for the Future

- Importing encounters, monsters and player characters from D&D Beyond.
- Simulating encounters.
- Terrain generation for battle maps and other advanced battle map features.
- Multithreaded application, mostly for rendering.
- Audio system, adding a background music/playlist to specific encounters.

The project is in early development, so the list above should be taken as a plan.

## Tech Stack

- C++23
- Dear ImGui
- OpenGL/GLFW/GLAD/GLM
- nlohmann/json
- stb_image
- GoogleTest

## Prerequisites

- A C++23 compiler
- OpenGL 3.3+
- CMake 3.25+
- Ninja
- Git

On Linux, GLFW needs the system windowing headers:

```bash
sudo apt install xorg-dev libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules
```

## Building

Clone the repository:

```bash
git clone https://github.com/dorukkolcak/CombatTracker.git
cd CombatTracker
```

Configure, build and test, all in one command:

```bash
cmake --workflow --preset release
```

Use `--preset debug` for a debug build.

To build with a specific compiler, set `CXX` first:

```bash
CXX=clang++ cmake --workflow --preset release           # bash
$env:CXX="clang++"; cmake --workflow --preset release   # PowerShell
```

On Windows, every compiler that targets the MSVC ABI needs the MSVC headers and libraries on `PATH`, which
covers `cl`, `clang-cl` and `clang++`. Run the command from a Developer Command Prompt when you pick one of
those. MinGW GCC needs no special shell, and neither does any compiler on Linux or macOS.

## Running

```bash
./build/release/src/CombatTrackerApp/Combat-Tracker-App
```

Dear ImGui writes an `imgui.ini` in the working directory to remember window positions. It is ignored by git
and can be deleted at any point to get the default layout back.

## Configuring

- `cmake --workflow` takes no `-D` options, so configure and build separately when you need one:
  `cmake --preset release -D BUILD_TESTING=OFF` followed by `cmake --build --preset release`.
- `-D BUILD_TESTING=OFF` skips fetching and building GoogleTest, which is worth doing for a faster first
  configure if you are not going to run the tests.
- `-D FETCHCONTENT_BASE_DIR=<path>` moves the fetched dependencies somewhere shared, so several build
  directories do not each download their own copy.
- Warnings are always enabled, but only become errors when the `CI` environment variable is set, which every
  CI provider does automatically. To reproduce that strictness locally, configure with
  `-D CMAKE_COMPILE_WARNING_AS_ERROR=ON`.
- `compile_commands.json` is written to the build directory for clangd and similar tooling.
- Personal preset overrides belong in `CMakeUserPresets.json`, which is ignored by git.
