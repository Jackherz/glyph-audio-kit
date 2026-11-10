# Glyph Audio panel kit

The shared code behind every Glyph Audio plug-in: one look, one preset system and one
way of passing audio between plug-ins. A plug-in picks its paper colour (a `kit::Stock`)
and gets the rest, so the same control looks and behaves the same wherever you meet it.

Used by [Silt](https://github.com/Jackherz/silt) and
[Brood](https://github.com/Jackherz/Brood).

## What's in it

| Header | What it gives a plug-in |
|---|---|
| [`Look.h`](include/kit/Look.h) | The sheet: one ink, two typefaces, hard offset shadows, square corners. A `LookAndFeel` for knobs, keys and fields, and drawing for panels, the masthead and the readout. Stocks: `cream()`, `graphite()` |
| [`Presets.h`](include/kit/Presets.h) | `PresetLibrary`, `PresetBrowser` and `PresetBar`: a built-in factory set, user presets as XML files in the standard preset folder, categories, favourites and search. The plug-in says how to capture and apply a sound |
| [`Bench.h`](include/kit/Bench.h) | The Bench: one plug-in sends a few bars of audio, another takes them. Works across processes and hosts; neither needs the other open |
| [`TapeInput.h`](include/kit/TapeInput.h) | Input gain past 0 dB as an old tape machine: a soft ceiling, more bass, less top and some hiss. Bit for bit at or below 0 dB |
| [`HalfBand.h`](include/kit/HalfBand.h) | Half-band filters for running a stage at twice the sample rate |
| [`fonts/`](fonts) | VT323 and Glyph Grotesque, embedded as the `kit_fonts` binary-data target |

Everything is header-only apart from the fonts. It needs JUCE 8 (`juce_gui_basics`,
`juce_audio_formats`).

## Using it

Add it to a plug-in's repository as a submodule at `kit/`:

```bash
git submodule add https://github.com/Jackherz/glyph-audio-kit.git kit
```

```cmake
add_subdirectory(kit)
target_link_libraries(MyPlugin PRIVATE kit_fonts)   # also puts include/ on the path
```

## Changing it

Change it here and push first. Then, in each plug-in that should move to the new version:

```bash
git submodule update --remote kit
git add kit && git commit -m "Move kit to <commit>"
```

Plug-ins move when they're ready, so two plug-ins can sit on different versions.

## Licence

The kit's code is licensed under the GNU GPL v3 ([LICENSE](LICENSE)), like the plug-ins
that build on it. It needs JUCE, which is used under its AGPLv3 licence. The fonts are under the
SIL Open Font License ([`fonts/OFL-VT323.txt`](fonts/OFL-VT323.txt),
[`fonts/OFL-GlyphGrotesque.txt`](fonts/OFL-GlyphGrotesque.txt)). Glyph Grotesque is
Terminal Grotesque by Raphael Bastide, renamed.
