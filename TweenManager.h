#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
#include "Tween.h"

namespace DekiTween
{

/// Holds and updates every tween created through the static API (a singleton).
///
/// The package hooks it into the engine's per-frame update (TweenInit.cpp),
/// and every TweenComponent's Update() drives it too; frame tracking makes it
/// run once per frame, whichever path comes first.
class DEKI_TWEEN_API TweenManager
{
public:
    static TweenManager& Instance();

    /// Advances every active tween by `deltaTimeSeconds`.
    void Update(float deltaTimeSeconds);

    /// Updates the tweens unless that already happened this frame (tracked
    /// with Deki::Time). The per-frame hook and TweenComponent::Update() both
    /// call it.
    void EnsureUpdatedThisFrame();

    void KillAll();

    /// Kills the tweens that write to `target`.
    template <typename T>
    void KillTweensOf(T* target);

    size_t GetActiveTweenCount() const { return m_Tweens.size(); }

    // ========== Static Factory API ==========

    /// Tweens from the target's current value to `endValue`. Durations are in seconds.
    static Tween<float>& To(float* target, float endValue, float duration);

    /// Tweens from `startValue` to `endValue`.
    static Tween<float>& FromTo(float* target, float startValue, float endValue, float duration);

    static Tween<int32_t>& To(int32_t* target, int32_t endValue, float duration);

    static Tween<int32_t>& FromTo(int32_t* target, int32_t startValue, int32_t endValue, float duration);

    static Tween<Deki::Vector2>& To(Deki::Vector2* target, const Deki::Vector2& endValue, float duration);

    static Tween<Deki::Vector2>& FromTo(Deki::Vector2* target, const Deki::Vector2& startValue,
                                        const Deki::Vector2& endValue, float duration);

    static Tween<Deki::Color>& To(Deki::Color* target, const Deki::Color& endValue, float duration);

    static Tween<Deki::Color>& FromTo(Deki::Color* target, const Deki::Color& startValue, const Deki::Color& endValue,
                                      float duration);

    /// Calls `callback` after `delay` seconds; nothing is interpolated.
    static Tween<float>& DelayedCall(float delay, std::function<void()> callback);

private:
    TweenManager();
    ~TweenManager();
    TweenManager(const TweenManager&) = delete;
    TweenManager& operator=(const TweenManager&) = delete;

    // Adds a tween and returns it, for chaining.
    template <typename T>
    Tween<T>& AddTween(std::unique_ptr<Tween<T>> tween);

    std::vector<std::unique_ptr<ITween>> m_Tweens;
    uint32_t m_NextId = 1;

    // Tweens added during Update(), held here so the loop's iterators stay valid
    std::vector<std::unique_ptr<ITween>> m_TweensToAdd;
    bool m_IsUpdating = false;

    // Frame tracking for EnsureUpdatedThisFrame()
    uint32_t m_LastUpdateTime = 0;
};

// ========== Templates ==========

template <typename T>
void TweenManager::KillTweensOf(T* target)
{
    for (auto& tween : m_Tweens)
    {
        auto* typedTween = dynamic_cast<Tween<T>*>(tween.get());
        if (typedTween && typedTween->GetTarget() == target)
        {
            typedTween->Kill();
        }
    }
}

template <typename T>
Tween<T>& TweenManager::AddTween(std::unique_ptr<Tween<T>> tween)
{
    tween->id = m_NextId++;
    Tween<T>& tweenRef = *tween;

    if (m_IsUpdating)
    {
        m_TweensToAdd.push_back(std::move(tween));
    }
    else
    {
        m_Tweens.push_back(std::move(tween));
    }

    return tweenRef;
}

}  // namespace DekiTween
