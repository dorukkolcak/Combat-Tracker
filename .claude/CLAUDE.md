# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository. This file should be seen as an overview and shouldn't include very specific/detailed info about a specific part of the codebase/project.

## Project Overview

Read the README.md file in the source folder to understand the purpose, features and how to build, run and configure the project. 
This document should be your starting point and baseline for understanding the project.

## Architecture

The source code of the project is split into two directories, each having its own namespace. The directories create two separate targets but use the same root folder for includes:

- **`src/CombatTrackerLib`** is the reusable library part of the project. 
This library is intentionally designed to be domain-agnostic, providing generic functionality and systems for any application.
Hence, any functionality that is associated with domain concepts whether for this project or any other application that links to the static library should not be put here.
Specifically, the library knows nothing about D&D; if a change makes the library aware of player characters for instance, it is in the wrong target.
Examples for systems and features that are suitable for this directory are as follows:
  - Windowing 
  - Scene management
  - Events
  - Audio 
  - Shader management 
  - Texture management 
  - UI layouting and styling
  - Many other domain-agnostic features

- **`src/CombatTrackerApp`** is the domain half of the project. All TTRPG specific concepts and the actual executable of the project reside here.
This part of the project links against the library part to utilize the generic functionality provided by the library.
Examples for systems and features that are suitable for this project are as follows:
	- Entity Model
	- Concrete Scene implementations having the proper logic for UI and event management
	- Specific data/file management logic for tasks such as parsing files that have entity informationI

- This architectural distinction is the single most important thing to consider during development and the boundaries listed should be respected at all times.

- **`scripts`** is where utilities that are not part of the core runtime application but are related to the development of the project should live. Some examples can be as follows:
  - Scripts for generating assets that are shipped with the project. The script itself shouldn't be part of the source code as we only care about the output of the script.
  - Scripts for logging, automation and setting up the project.

- **`tests`** is where unit tests for the source code live. The directory is further split into two subdirectories: CombatTrackerLib and CombatTrackerApp. 
Unit tests exist for both the library-side and the application side of the project. Hence, tests written in one subdirectory should only test functionality for the corresponding part of the project.

- **`external`** is where external dependencies live. This should almost never be touched by you and should be configured by the actual developer.

- One thing to keep in mind is that the examples given for directories shouldn't be seen as strict rules here. There can be many examples that fit one part of the project.
When adding a feature, the guidelines in this section should be taken into consideration and the feature should be categorized into the proper part of the project.

## Conventions and Style

- `src/` is the only include directory, so every header has exactly one spelling. Relative includes are not used anywhere and should not be introduced.

- Prefer a modern C++ approach to implementation. This doesn't mean that you should blindly use modern C++ features for the sake of using them. But among viable solutions to a given problem, prefer modern C++ solutions, utilizing standard library features to older conventions. For unfamiliar modern C++ features/APIs, you should in general give me an overview of the feature/API, why and how to use it so that I can understand the positives and tradeoffs.

- There may be cases where you have to use older, C-style C++ code due to external APIs. If there are no viable alternatives, you can use solutions such as these. 

- Use `m_` members, `s_` function-local statics and `UPPER_SNAKE` namespace-wide constants across the codebase. Use camel-case variable and function naming style. Never use global variables and `g_` naming conventions.

- Always utilize the corresponding namespaces when adding code. Never utilize unnamed namespaces.

- Prefer brace initialization in general. If a type has a constructor that takes an initializer list argument, you should use direct brace initialization in that case, if not intending to use the initializer list constructor itself.

- The codebase is configured so that the compilers treat all warnings as errors. So the generated code should produce no warnings.

- Comments should only be written by the actual developer. Preserve all comments when editing code and never write comments yourself. If the developer asks you to add explicit documentation for your work, you should make a write up in another form. You can delete/modify/correct redundant/outdated/unnecessary comments due to changes to the codebase.

- It is really important that you respect the given style/convention guidelines and visible conventions/stylistic choices already available in the code. Additions that do not conform to these guidelines/visible stylistic choices shouldn't be made at all.

- The project is developed in a very professional mindset. Employ this mindset while developing the project and have quality as the main criteria.

## Other Guidelines

- Architectural decisions should be made by the developer. You can give suggestions but the final decision on how to organize code and layers should be made by the developer.

- You can suggest features for the project if you see them fit. But the final decision to develop which features should always be made by the developer.

- You can suggest developer tooling that you think fits. These suggestions should be done with respect to professionalism. The tools should be widely adopted for serious C++ development and large scale project management. You should give a rigorous description on how to adopt said tools during initial configuration.

- Scoped enumerators are ordered to follow an arbitrary order based on the rulebook, and their integral values are the persistence format. Entity save files and the localization files both key domain values by ordinal, which makes enum ordering a data contract rather than an implementation detail. New enumerators are always appended immediately before `COUNT`, never inserted or reordered, and an enumerator is never removed. Breaking this silently reinterprets every save file on disk and shifts every localization entry without raising an error anywhere. Any change to an existing enum's ordering has to be treated as a save format migration.

- Localization lives in `assets/Locales/<localeCode>.json`, one file per locale, selected through `App::Specification::locale`. Each file carries three top level sections: `locale` for metadata, `scenes` mirroring the scene structure for UI strings, and `domain` for the arrays that are indexed by enumerator ordinal as described above. `LocaleProvider` parses the chosen file once and holds `std::string_view` maps into it, so `getText` and `getTextList` cost no allocation but return non-owning views that stay valid only until the next `setLocale`. Keys are JSON pointers and nothing validates them, as a missing key makes `getText` return the key itself and surface a raw pointer path on screen rather than fail, so every locale file has to carry an identical key structure and identical list lengths. Values are always complete sentences instead of templates with runtime placeholders, because grammar past the nominative differs per entity in inflected languages, which means a shared widget receives its already resolved strings from the caller rather than substituting a name into a pattern. Localized text is also never used as an ImGui widget id; ids come from a `###` suffix inside the string or from a separate non-localized id parameter, otherwise widget state resets whenever the language changes.
