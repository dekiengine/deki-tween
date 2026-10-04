#pragma once

/// Hooks the tween manager into the engine's per-frame update.
///
/// Called from DekiInitPackageSystems() on firmware and static builds and from
/// DekiPluginInit() when the package is loaded as a DLL. Without it, tweens
/// started through the TweenManager API would advance only while some
/// TweenComponent exists in the scene.
///
/// Safe to call more than once.
///
/// GLOBAL SCOPE on purpose, the one part of this package that is. These are
/// link-time glue: the editor generates a translation unit that declares them
/// as plain `extern void DekiTweenInitSystem();` and calls them to start a
/// static simulator or firmware build. That generated file cannot know a
/// package's namespace, so the symbol carries the package prefix itself, as
/// DekiTweenRegisterComponents from the reflection codegen does. Keep them
/// here, outside the namespace.
void DekiTweenInitSystem();
void DekiTweenShutdownSystem();
