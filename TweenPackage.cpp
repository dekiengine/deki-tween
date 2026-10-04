/**
 * @file TweenPackage.cpp
 * @brief Package entry point for deki-tween DLL
 *
 * This file exports the standard Deki plugin interface so the editor
 * can load deki-tween.dll and register its components (TweenComponent).
 *
 * For linked DLLs (not dynamically loaded), DekiTweenEnsureRegistered()
 * must be called from the main executable to trigger the static initializers.
 */

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
// When deki-tween is linked (not dynamically loaded), the editor must call
// this function to ensure the DLL code is actually loaded and the static
// initializers (REGISTER_COMPONENT) have run.

// Auto-generated registration helpers

// Track if already registered to avoid duplicates
static bool s_TweenRegistered = false;

// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiTween;

extern "C"
{
    /**
     * @brief Ensure deki-tween package is loaded and components are registered
     *
     * Call this from the editor at startup. Simply calling this function is enough
     * to force the linker to include the DLL and trigger static initializers.
     *
     * @return Number of components registered by this package
     */
    DEKI_TWEEN_API int DekiTweenEnsureRegistered(void)
    {
        if (s_TweenRegistered)
        {
            return ::DekiTweenGetAutoComponentCount();
        }
        s_TweenRegistered = true;

        // Auto-generated: registers all Tween components with ComponentRegistry + ComponentFactory
        ::DekiTweenRegisterComponents();

        return ::DekiTweenGetAutoComponentCount();
    }

}  // extern "C"

// =============================================================================
// Plugin metadata (for dynamic loading compatibility)
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
    // Package-specific feature API (for linked DLL access without name conflicts)
    // =============================================================================

    DEKI_TWEEN_API const char* DekiTweenGetName(void)
    {
        return "Tween";
    }

}  // extern "C"

#endif  // DEKI_EDITOR
}  // namespace DekiTween
