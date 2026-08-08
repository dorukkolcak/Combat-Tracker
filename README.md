# CombatTracker

CombatTracker is a personal tool for running my own D&D 5e games. The tool keeps track of encounters on a
turn-by-turn basis, keeping track of the Actions/Bonus Actions/Reactions of every single monster and player
character in an encounter. It also handles creating and saving the encounters themselves, along with the
monsters and player characters that go into them.

As a future extension, the tool will have a battlefield visualization for each encounter where every player
character and monster can be moved around a square-grid battle map with elevation and cover features. The
rendering is planned to be just wireframe rendering, possibly using an exported jpg/png of a battle map as a
texture.

## Why This Exists

Frankly there are a LOT of alternatives to a tool like this (the most popular is probably
[improvedinitiative.app](https://improvedinitiative.app)). The main reasons I wanted to write one anyway are as
follows:

1. Programming and software design practice.
2. I have some homebrew rules/mechanics I use in my games, Unleash Actions being the major one. None of the
   existing tools can support tracking my homebrew mechanics. This matters especially for Unleash Actions, as
   they are typically once per combat actions that progress throughout an entire encounter.
3. It is much more convenient to use a tool I designed myself than to use an external one. Many D&D tools also
   have quite steep learning curves, and frankly speaking, I am not interested in spending the time to learn
   one of them. (But I am somehow interested in creating a tool from scratch...)

So this is a personal tool and not a product. That is worth keeping in mind while reading the rest of this
document, as it explains a good number of the decisions made here.

## Features

### Baseline

- Encounter creation, management and saving.
- Monster and player character creation and saving.
- Starting and running encounters, tracking them on a turn-by-turn basis.
- Interactive encounter battle map visualization, either 2D or 3D.
- Viewing completed encounters.
- Other custom and homebrew mechanic support.

### Far Future

- Importing encounters, monsters and player characters from D&D Beyond.
- Simulating encounters.
- Terrain generation for battle maps and other advanced battle map features.
- Multithreaded application, mostly for rendering.
- Audio system, adding a background music/playlist to specific encounters.

### How The UI Is Meant To Be Laid Out

All of the functionality for creating and running encounters should ideally live on a single
viewport/layer/scene. Rolling or setting initiative, playing each turn, visualizing the battle map and marking
encounters as completed should all be done from a single screen. The same goes for encounter, player character
and monster creation, management, saving and editing, and for seeing the encounters marked as completed.

So in total there are three "main scenes", each of which is accessible through a main menu. This is a
requirement rather than an implementation detail, and new UI work should fit into it instead of adding a
fourth top-level screen.

## Current State

The project is in early development, so the list above is a plan and not a description of what runs today.
What currently works:

- The main menu, including theme selection.
- Monster and player character creation, along with saving, loading, editing and deleting them as JSON.
- The encounter creation scene is reachable from the main menu, but is still being built out.

The "Select Encounter" and "Browse Encounters" buttons on the main menu are deliberately inert for now, as the
scenes behind them do not exist yet.

## Tech Stack

- C++23
- Dear ImGui for the UI
- OpenGL/GLFW/GLAD/GLM for windowing and rendering
- nlohmann/json for persistence
- stb_image for texture loading
- GoogleTest for unit tests

GLFW, GLM, nlohmann/json and GoogleTest are pulled in by CMake, preferring an installed copy where one exists
and falling back to `FetchContent`. GLAD, Dear ImGui and stb_image are vendored under `external/`. All of the
third-party include directories are marked `SYSTEM`, so the warning level used for this project does not apply
to headers that cannot be fixed here anyway.

## Building

The build uses CMake presets with the Ninja generator, and requires CMake 3.24 or newer.

### Prerequisites

**Windows.** Visual Studio 2022 with the C++ toolchain, plus Ninja (it ships with the VS installer). The
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

The `release` presets are `RelWithDebInfo` rather than `Release`, since a personal tool benefits far more from
having usable stack traces than from the last few percent of performance.

Warnings are treated as errors, but only for the targets belonging to this project. On MSVC the build uses
`/W4 /utf-8 /w15038` and on GCC/Clang it uses `-Wall -Wextra`. `/w15038` is the MSVC counterpart of
`-Wreorder`, which means member initializer lists have to follow declaration order or the Linux and macOS
builds break.

## Running

```
out/build/windows-debug/src/CombatTrackerApp/Combat-Tracker-App.exe
```

**Run the application from the repository root.** `assets/` is copied next to the executable as a post-build
step, so the title texture is found no matter where the application is launched from. `data/` is not, and is
still relative to the working directory, so launching from inside the build directory silently loses every
saved monster and player character. Nothing reports an error in that case, which makes it a confusing thing to
debug if you do not know to look for it.

Moving off working-directory relative paths is therefore half done. `MainMenuScene` resolves the title texture
through `App::getResourceDirectory()`, which points at the executable's own directory.
`App::getUserDataDirectory()` resolves the writable per-user location on all three platforms, but it has no
call sites yet, so `EntityStorage.h` still uses `data/Monsters` and `data/PlayerCharacters` as written.

The title texture lives at `assets/Title.png` and is generated rather than hand-drawn. If it is missing,
regenerate it with `scripts/MakeTitle.ps1` (Windows only, since it goes through GDI+):

```
powershell -ExecutionPolicy Bypass -File scripts/MakeTitle.ps1
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

## Repository Layout

| Directory | Contents |
| --- | --- |
| `src/CombatTrackerLib` | The domain-agnostic library half of the project. |
| `src/CombatTrackerApp` | The D&D specific half, and the actual executable. |
| `tests/` | Unit tests, split to mirror the two source directories. |
| `external/` | Vendored dependencies: GLAD, Dear ImGui and stb_image. |
| `scripts/` | Development utilities whose output is what matters, not the script itself. |
| `assets/` | Read-only assets shipped with the application. |
| `data/` | Saved monsters and player characters, written as JSON at runtime. |

`src/` is the only include directory, so every header has exactly one spelling, such as
`#include "CombatTrackerLib/Utilities/UIUtils.h"`. Relative includes are not used anywhere.

## Continuous Integration

`.github/workflows/build.yml` configures, builds and tests all six presets on every push and pull request. The
matrix does not fail fast, because the entire point of it is to see the result for every platform on a single
run. The GCC and Clang legs are the only place `-Wall -Wextra` is actually exercised, so a change that only
ever gets built on Windows can still break there.

## License

MIT, see `LICENSE.txt`.
