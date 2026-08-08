<#
.SYNOPSIS
    Generates the CombatTracker wordmark PNG using GDI+ (no external tooling).
	This is a Windows only script, quite obviously.
    Also for disclosure this script is completely AI generated as I only needed the script's output.

.DESCRIPTION
    Renders a two-tone wordmark on a transparent background, optionally underlined
    by a rule broken with a diamond (the divider motif from a 5e stat block).

    Defaults match the DARK preset in src/CombatTrackerLib/Themes.cpp, so the logo
    stays in the UI palette instead of sitting next to it.

    Windows-only: System.Drawing wraps GDI+. Run it whenever the wordmark needs to
    change so the asset stays reproducible rather than being an opaque binary.

.EXAMPLE
    .\make_title.ps1
    Regenerates assets/Title.png with the shipped defaults.

.EXAMPLE
    .\make_title.ps1 -PrimaryText "Encounter" -AccentText "Builder" -FontSize 72 -OutFile ..\assets\Builder.png

.EXAMPLE
    .\make_title.ps1 -AccentColor "#D4AF37" -NoRule -Autosize
    Gold accent, no divider, canvas cropped to the text.
#>
[CmdletBinding()]
param(
    # The wordmark is split into two runs so the camelCase boundary can be coloured.
    [string] $PrimaryText   = "Combat",
    [string] $AccentText    = "Tracker",

    [string] $PrimaryColor  = "#DCE2E8",   # ImGuiCol_Text
    [string] $AccentColor   = "#C85554",   # ImGuiCol_Button
    [string] $RuleColor     = "#C85554",   # matches the accent by default
    [string] $DiamondColor  = "#D4AF37",   # ImGuiCol_SliderGrab
    [int]    $ShadowAlpha   = 150,         # 0 disables the drop shadow

    # Optional tagline under the wordmark. Empty means no subtitle at all.
    [string] $SubtitleText      = "",
    [string] $SubtitleColor     = "#DCE2E8",   # ImGuiCol_Text
    [int]    $SubtitleFontSize  = 30,

    [string] $FontName      = "Bookman Old Style",
    [int]    $FontSize      = 96,
    [switch] $NoBold,

    [int]    $Width         = 1024,
    [int]    $Height        = 256,
    # Ignore -Width/-Height and crop the canvas to the rendered content instead.
    [switch] $Autosize,
    [int]    $Padding       = 48,

    [switch] $NoRule,

    [string] $OutFile = (Join-Path $PSScriptRoot "..\assets\Title.png")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.Drawing

function ConvertTo-Color {
    param([string] $Hex, [int] $Alpha = 255)

    $h = $Hex.TrimStart('#')
    if ($h.Length -ne 6) {
        throw "Colour '$Hex' must be 6 hex digits, e.g. '#C85554'."
    }
    $r = [Convert]::ToInt32($h.Substring(0, 2), 16)
    $g = [Convert]::ToInt32($h.Substring(2, 2), 16)
    $b = [Convert]::ToInt32($h.Substring(4, 2), 16)
    return [System.Drawing.Color]::FromArgb($Alpha, $r, $g, $b)
}

# GDI+ resources are unmanaged; collect them so a single finally can release them all.
$disposables = @()
function Track { param($Resource) ; $script:disposables += $Resource ; return $Resource }

try {
    $bold = [System.Drawing.FontStyle]::Bold
    if ($NoBold) { $bold = [System.Drawing.FontStyle]::Regular }
    $font = Track (New-Object System.Drawing.Font($FontName, $FontSize, $bold, [System.Drawing.GraphicsUnit]::Pixel))

    if ($font.Name -ne $FontName) {
        Write-Warning "Font '$FontName' is not installed; GDI+ substituted '$($font.Name)'."
    }

    # The tagline reads better unemphasised, so it stays regular weight regardless of -NoBold.
    $hasSubtitle  = -not [string]::IsNullOrWhiteSpace($SubtitleText)
    $subtitleFont = $null
    if ($hasSubtitle) {
        $subtitleFont = Track (New-Object System.Drawing.Font($FontName, $SubtitleFontSize, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel))
    }

    # GenericTypographic measures tight glyph extents; the default format pads for
    # legacy layout and would leave the wordmark visibly off-centre.
    $fmt = Track (New-Object System.Drawing.StringFormat([System.Drawing.StringFormat]::GenericTypographic))
    $fmt.FormatFlags = $fmt.FormatFlags -bor [System.Drawing.StringFormatFlags]::MeasureTrailingSpaces

    # Measuring needs a Graphics, but the real canvas size may depend on the result,
    # so measure against a throwaway surface first.
    $probeBmp = Track (New-Object System.Drawing.Bitmap(1, 1))
    $probe    = Track ([System.Drawing.Graphics]::FromImage($probeBmp))
    $probe.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::AntiAlias

    $wPrimary = $probe.MeasureString($PrimaryText, $font, [int]::MaxValue, $fmt).Width
    $wAccent  = $probe.MeasureString($AccentText,  $font, [int]::MaxValue, $fmt).Width
    $wTotal   = $wPrimary + $wAccent
    $hText    = $probe.MeasureString("$PrimaryText$AccentText", $font, [int]::MaxValue, $fmt).Height

    $wSubtitle = 0.0
    $hSubtitle = 0.0
    if ($hasSubtitle) {
        $wSubtitle = $probe.MeasureString($SubtitleText, $subtitleFont, [int]::MaxValue, $fmt).Width
        $hSubtitle = $probe.MeasureString($SubtitleText, $subtitleFont, [int]::MaxValue, $fmt).Height
    }

    # The rule and tagline sit below the wordmark; reserve room only when drawn.
    $ruleBlock = 0.0
    if (-not $NoRule) { $ruleBlock = 54.0 }
    $subtitleBlock = 0.0
    if ($hasSubtitle) { $subtitleBlock = $hSubtitle + 20.0 }

    # A long tagline can be wider than the wordmark, so fit to whichever dominates.
    $wWidest = [math]::Max($wTotal, $wSubtitle)

    if ($Autosize) {
        $Width  = [int][math]::Ceiling($wWidest + (2 * $Padding))
        $Height = [int][math]::Ceiling($hText + $ruleBlock + $subtitleBlock + (2 * $Padding))
    }
    elseif ($wWidest -gt ($Width - (2 * $Padding))) {
        Write-Warning ("Text is {0}px wide but the canvas allows {1}px. Lower -FontSize, or pass -Autosize." -f `
                       [math]::Round($wWidest), ($Width - (2 * $Padding)))
    }

    $bmp = Track (New-Object System.Drawing.Bitmap($Width, $Height, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb))
    $g   = Track ([System.Drawing.Graphics]::FromImage($bmp))
    $g.Clear([System.Drawing.Color]::Transparent)
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
    # Grayscale AA, not ClearType: subpixel AA writes colour fringes that read as
    # magenta/green haloes once the image is composited over an arbitrary background.
    $g.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::AntiAlias

    $x = ($Width - $wTotal) / 2.0
    $y = ($Height - $hText - $ruleBlock - $subtitleBlock) / 2.0

    if ($ShadowAlpha -gt 0) {
        $brShadow = Track (New-Object System.Drawing.SolidBrush((ConvertTo-Color "#000000" $ShadowAlpha)))
        $g.DrawString($PrimaryText, $font, $brShadow, ($x + 4), ($y + 4), $fmt)
        $g.DrawString($AccentText,  $font, $brShadow, ($x + $wPrimary + 4), ($y + 4), $fmt)
    }

    $brPrimary = Track (New-Object System.Drawing.SolidBrush((ConvertTo-Color $PrimaryColor)))
    $brAccent  = Track (New-Object System.Drawing.SolidBrush((ConvertTo-Color $AccentColor)))
    $g.DrawString($PrimaryText, $font, $brPrimary, $x, $y, $fmt)
    $g.DrawString($AccentText,  $font, $brAccent, ($x + $wPrimary), $y, $fmt)

    if (-not $NoRule) {
        $ruleY = $y + $hText + 18.0
        $xL    = $x - 18.0
        $xR    = $x + $wTotal + 18.0
        $mid   = ($xL + $xR) / 2.0
        $gap   = 22.0

        $pen = Track (New-Object System.Drawing.Pen((ConvertTo-Color $RuleColor), 5))
        $pen.StartCap = [System.Drawing.Drawing2D.LineCap]::Round
        $pen.EndCap   = [System.Drawing.Drawing2D.LineCap]::Round
        $g.DrawLine($pen, $xL, $ruleY, ($mid - $gap), $ruleY)
        $g.DrawLine($pen, ($mid + $gap), $ruleY, $xR, $ruleY)

        $d = 11.0
        $diamond = @(
            (New-Object System.Drawing.PointF($mid, ($ruleY - $d))),
            (New-Object System.Drawing.PointF(($mid + $d), $ruleY)),
            (New-Object System.Drawing.PointF($mid, ($ruleY + $d))),
            (New-Object System.Drawing.PointF(($mid - $d), $ruleY))
        )
        $brDiamond = Track (New-Object System.Drawing.SolidBrush((ConvertTo-Color $DiamondColor)))
        $g.FillPolygon($brDiamond, $diamond)
    }

    if ($hasSubtitle) {

        # Sits clear of the rule when there is one, otherwise tucks under the wordmark.
        $subtitleY = $y + $hText + $ruleBlock
        if ($NoRule) { $subtitleY = $y + $hText + 12.0 }

        # Centred on its own width: the tagline rarely matches the wordmark's width.
        $subtitleX = ($Width - $wSubtitle) / 2.0

        $brSubtitle = Track (New-Object System.Drawing.SolidBrush((ConvertTo-Color $SubtitleColor)))
        $g.DrawString($SubtitleText, $subtitleFont, $brSubtitle, $subtitleX, $subtitleY, $fmt)
    }

    $outDir = Split-Path -Parent $OutFile
    if (-not (Test-Path $outDir)) {
        New-Item -ItemType Directory -Force $outDir | Out-Null
    }

    # Resolve to an absolute path: GDI+ saves relative to the process working
    # directory, which is not necessarily where the script lives. GetFullPath also
    # collapses the '..' in the default -OutFile.
    if ([System.IO.Path]::IsPathRooted($OutFile)) {
        $resolved = [System.IO.Path]::GetFullPath($OutFile)
    }
    else {
        $resolved = [System.IO.Path]::GetFullPath((Join-Path (Get-Location).ProviderPath $OutFile))
    }
    $bmp.Save($resolved, [System.Drawing.Imaging.ImageFormat]::Png)

    Write-Output ("Wrote {0} ({1}x{2}, text {3}px wide, font '{4}' @ {5}px)" -f `
                  $resolved, $Width, $Height, [math]::Round($wTotal), $font.Name, $FontSize)
}
finally {
    # Reverse order: Graphics before the Bitmap it draws into.
    [array]::Reverse($disposables)
    foreach ($d in $disposables) {
        if ($null -ne $d) { $d.Dispose() }
    }
}
