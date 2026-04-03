# Module descriptor for deki-engine auto-discovery
set(MODULE_DISPLAY_NAME "Tween")
set(MODULE_PREFIX "DekiTween")
set(MODULE_UPPER "TWEEN")
set(MODULE_TARGET "deki-tween")
set(MODULE_FILE_PREFIX "Tween")
set(MODULE_SOURCES
    Easing.cpp
    Tween.cpp
    TweenManager.cpp
    TweenComponent.cpp
)
set(MODULE_ENTRY TweenModule.cpp)
set(MODULE_EDITOR_SOURCES
    editor/TweenCustomEditor.cpp
)
set(MODULE_NEEDS_IMGUI ON)
set(MODULE_LINK_DEPS deki-editor)
set(MODULE_EXPORT_ALL_SYMBOLS OFF)
