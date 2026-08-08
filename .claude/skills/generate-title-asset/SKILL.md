---
name: generate-title-asset
description: Regenerate assets/Title.png, the CombatTracker wordmark, using scripts/MakeTitle.ps1. Use when the title texture is missing from the working tree, or when the wordmark's text, font, size or layout should change.
---

# Generating the Asset

`assets/Title.png` is the wordmark drawn by `MainMenuScene`. It is generated rather than hand-drawn so it stays
reproducible instead of being an opaque binary that nobody can edit. `scripts/MakeTitle.ps1` is the generator.

```
powershell -ExecutionPolicy Bypass -File scripts/MakeTitle.ps1
```

The output path resolves from the script's own location rather than the working directory, so this writes to
`assets/Title.png` from anywhere, and it creates `assets/` if it is not there.

## Asset Specifications

Everything the script draws is pure white, and the shape lives entirely in the alpha channel. That is because
`MainMenuScene::renderUI` tints it at draw time:

```cpp
CombatTrackerLib::centeredImage(m_titleTexture, ImGui::GetStyle().Colors[ImGuiCol_Text]);
```

ImGui multiplies the texture by that tint, and white multiplied by a tint is the tint, so the logo comes out in
the active theme's text colour and follows the UI preset for free. Any colour baked into the PNG would instead
multiply with the tint and produce something that is neither the baked colour nor the theme colour.
The script has no colour parameters, so this is not something that can be got wrong by passing the wrong flag.

## Parameters

- **Text.** `-Text`, and `-SubtitleText` for an optional tagline underneath, empty by default.
- **Type and layout.** `-FontName`, `-FontSize`, `-SubtitleFontSize`, `-NoBold`, `-Width`, `-Height`,
  `-Padding`, `-Autosize`.
- **Ornament.** `-NoRule` removes the divider and the diamond that breaks it. The divider is the stat block
  motif from 5e, so dropping it is a real design change rather than a layout tweak.
- **Output.** `-OutFile` for writing a variant somewhere other than the shipped asset.

## Windows Only

The script draws through GDI+ by way of `System.Drawing`. There is no Linux or macOS path, and the asset is
committed to the repository precisely so the other two platforms never need to generate it. If it needs
regenerating on a non-Windows machine, say so rather than substituting ImageMagick or another tool, because the
output would no longer match.

## Verifying the Result

The script prints the resolved path, the canvas dimensions, the measured text width, the font it actually used
and the font size. Two things there are worth checking:

- **The font it actually used.** GDI+ silently substitutes when the requested font is not installed, and the
  script warns when it happens. The default is `Bookman Old Style`. A substitution means the wordmark no longer
  matches the committed asset.
- **A width warning.** If the text is wider than the canvas allows, the script warns rather than failing and
  writes a clipped image. Either lower `-FontSize` or pass `-Autosize` to crop the canvas to the content.

To check the result in the application, rebuild first. `assets/` is copied next to the executable by a
`POST_BUILD` step in `src/CombatTrackerApp/CMakeLists.txt`, and that step only fires when the target itself
relinks. Regenerating the PNG on its own changes nothing that Ninja considers a dependency, so the application
keeps loading the previous copy until something forces a link.
