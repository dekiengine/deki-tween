#pragma once

/**
 * @brief Hook the tween manager into the engine's per-frame update.
 *
 * Called from deki_init_package_systems() on firmware/static builds and from
 * DekiPlugin_Init() when the package is loaded as a DLL. Without it, tweens
 * started through the programmatic TweenManager API only advanced while some
 * TweenComponent happened to exist in the scene — and silently froze otherwise.
 *
 * Idempotent; safe to call more than once.
 */
void DekiTween_InitSystem();
void DekiTween_ShutdownSystem();
