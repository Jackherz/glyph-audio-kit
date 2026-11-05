# The Glyph Audio panel kit

One look and one set of gestures for every plug-in in the Glyph Audio range: same ink, type
and grid on a different paper stock. A plug-in supplies a `kit::Stock` and gets the knob, keys,
fields, panels, masthead, readout, preset browser and the Bench, which passes audio between
plug-ins.

Each plug-in's repository holds this one as a submodule at `kit/`:

    git submodule add https://github.com/Jackherz/glyph-audio-kit.git kit
    add_subdirectory(kit)
    target_link_libraries(MyPlugin PRIVATE kit_fonts)

Change it here and push, then in each plug-in run
`git submodule update --remote kit`, and commit the new position. Plug-ins move to a new kit
when they're ready.

VT323 and Press Start 2P are under the SIL Open Font License (`fonts/`). The kit's own licence
is still to be decided, and has to suit every plug-in that builds on it.
