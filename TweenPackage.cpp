// Package entry point of the deki-tween DLL: exports the standard Deki plugin
// interface so the editor can load it and register its components
// (TweenComponent).
//
// When the DLL is linked instead of loaded at run time, the main executable
// must call DekiTweenEnsureRegistered() to run the static initializers.

#include <deki/interop/Plugin.h>
#include "TweenComponent.h"
#include "TweenManager.h"
#include "TweenInit.h"
#include <deki/reflection/ComponentRegistry.h>
#include <deki/reflection/ComponentFactory.h>

extern void DekiTweenRegisterComponents();
extern int DekiTweenGetAutoComponentCount();
extern const Deki::ComponentMeta* DekiTweenGetAutoComponentMeta(int index);

namespace DekiTween
{

#ifdef DEKI_EDITOR

// =============================================================================
// Linked DLL initialization
// =============================================================================
// When deki-tween is linked, not loaded at run time, the editor calls
// DekiTweenEnsureRegistered so the DLL is really loaded and its static
// initializers (REGISTER_COMPONENT) have run.

// Set once registered, so a second call does not register twice.
static bool s_TweenRegistered = false;

// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiTween;

extern "C"
{
    /// Registers the package's components once and returns how many there
    /// are. Call from the editor at startup: the call alone makes the linker
    /// include the DLL and run its static initializers.
    DEKI_TWEEN_API int DekiTweenEnsureRegistered(void)
    {
        if (s_TweenRegistered)
        {
            return ::DekiTweenGetAutoComponentCount();
        }
        s_TweenRegistered = true;

        // Generated: registers every Tween component with ComponentRegistry and ComponentFactory.
        ::DekiTweenRegisterComponents();

        return ::DekiTweenGetAutoComponentCount();
    }

}  // extern "C"

// =============================================================================
// Plugin metadata, for loading as a DLL
// =============================================================================

extern "C"
{
    DEKI_PLUGIN_API const char* DekiPluginGetName(void)
    {
        return "Deki Tween Package";
    }

    DEKI_PLUGIN_API const char* DekiPluginGetVersion(void)
    {
#ifdef DEKI_PACKAGE_VERSION
        return DEKI_PACKAGE_VERSION;
#else
        return "0.0.0-dev";
#endif
    }

    DEKI_PLUGIN_API int DekiPluginInit(void)
    {
        // Ticks TweenManager from the engine's update loop, so programmatic tweens
        // run without a TweenComponent in the scene.
        DekiTweenInitSystem();
        return 0;
    }

    DEKI_PLUGIN_API void DekiPluginShutdown(void)
    {
        DekiTweenShutdownSystem();
        s_TweenRegistered = false;
    }

    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return ::DekiTweenGetAutoComponentCount();
    }

    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int index)
    {
        return ::DekiTweenGetAutoComponentMeta(index);
    }

    DEKI_PLUGIN_API void DekiPluginRegisterComponents(void)
    {
        DekiTweenEnsureRegistered();
    }

    DEKI_PLUGIN_API void DekiPluginOnPlayModeStop(void)
    {
        DekiTween::TweenManager::Instance().KillAll();
    }

    // deki-tween renders no editor UI of its own, so it links no ImGui and shares no
    // ImGui context. Its component inspectors are drawn by the editor via reflection.

    // =============================================================================
    // Package-specific API, named so linked DLLs do not clash
    // =============================================================================

    DEKI_TWEEN_API const char* DekiTweenGetName(void)
    {
        return "Tween";
    }

}  // extern "C"

#endif  // DEKI_EDITOR
}  // namespace DekiTween
