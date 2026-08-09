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

## Building

The build uses CMake presets with the Ninja generator, and requires CMake 3.24 or newer.

### Prerequisites

**Windows:** Visual Studio 2022 with the C++ toolchain, plus Ninja (it ships with the VS installer). The
Windows presets pin `cl.exe`, so **every CMake command has to run from a Developer shell**. Either launch a
"x64 Native Tools Command Prompt for VS 2022" or run `vcvars64.bat` in the shell first. Without this, the
configure step fails to find the compiler, which is easily the most common way to have the build not work.

**Linux.** A C++23 capable GCC or Clang, Ninja, and the development headers GLFW needs. GLFW builds both its
X11 and Wayland backends, so both sets have to be present:

```
sudo apt-get install -y ninja-build xorg-dev libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules
```

**macOS.** The Xcode command line tools and Ninja (`brew install ninja`).

### Configure, Build And Test

There are six presets, one per platform and configuration: `windows-debug`, `windows-release`, `linux-debug`,
`linux-release`, `macos-debug` and `macos-release`. Each configure preset has a matching build and test preset
under the same name.

```
cmake --preset windows-debug            # configure
cmake --build --preset windows-debug    # build
ctest --preset windows-debug            # test
```

## Running

```
out/build/windows-debug/src/CombatTrackerApp/Combat-Tracker-App.exe
```

Dear ImGui writes an `imgui.ini` next to the working directory to remember window positions. It is ignored by
git and can be deleted at any point to get the default layout back.

## Configuring

- `-D BUILD_TESTING=OFF` skips fetching GoogleTest entirely, which is worth doing for a faster first configure
  if you are not going to run the tests.
- `-D FETCHCONTENT_BASE_DIR=<path>` moves the fetched dependencies somewhere shared, so several build
  directories do not each download their own copy. CI uses this to cache them between runs.
- `CMAKE_EXPORT_COMPILE_COMMANDS` is on, so a `compile_commands.json` lands in the build directory for clangd
  and similar tooling.
- Personal preset overrides belong in `CMakeUserPresets.json`, which is ignored by git.
