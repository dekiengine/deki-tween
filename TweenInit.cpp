#include "TweenInit.h"
#include "TweenManager.h"
#include "DekiEngine.h"
#include "DekiLogSystem.h"

#include <cstdint>

namespace
{
constexpr size_t kNotRegistered = SIZE_MAX;
size_t s_UpdateId = kNotRegistered;
}  // namespace

void DekiTween_InitSystem()
{
    if (s_UpdateId != kNotRegistered)
        return;

    // EnsureUpdatedThisFrame() rather than Update(): it is frame-guarded, so a
    // TweenComponent that also ticks the manager this frame does not advance
    // every tween twice.
    s_UpdateId = DekiEngine::GetInstance().RegisterUpdate(
        [](uint32_t /*deltaTimeMs*/)
        { Deki::TweenManager::Instance().EnsureUpdatedThisFrame(); });

    DEKI_LOG_INTERNAL("DekiTween: tween manager hooked into the engine update");
}

void DekiTween_ShutdownSystem()
{
    if (s_UpdateId == kNotRegistered)
        return;

    // Must happen before the DLL unloads: the callback's code lives here.
    DekiEngine::GetInstance().UnregisterUpdate(s_UpdateId);
    s_UpdateId = kNotRegistered;
    Deki::TweenManager::Instance().KillAll();
}
