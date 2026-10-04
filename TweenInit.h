#pragma once

/**
 * @brief Hook the tween manager into the engine's per-frame update.
 *
 * Called from DekiInitPackageSystems() on firmware/static builds and from
 * DekiPluginInit() when the package is loaded as a DLL. Without it, tweens
 * started through the programmatic TweenManager API only advanced while some
 * TweenComponent happened to exist in the scene — and silently froze otherwise.
 *
 * Idempotent; safe to call more than once.
 *
 * GLOBAL SCOPE, deliberately, and the one part of this package that is.
 * These are link-time glue: the editor generates a translation unit that
 * declares them as plain `extern void DekiTweenInitSystem();` and calls them
 * to bring a static simulator or firmware build up. That generated file cannot
 * know a package's namespace, which is why the symbol carries the package
 * prefix itself, as DekiTweenRegisterComponents from the reflection codegen
 * does. Keep them here, outside the namespace.
 */
void DekiTweenInitSystem();
void DekiTweenShutdownSystem();
