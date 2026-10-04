#pragma once

#include <cstdint>
#include <functional>
#include "Easing.h"
#include <deki/Vector.h>
#include <deki/Color.h>

namespace DekiTween
{

enum class TweenState : uint8_t
{
    Idle,      // Not started
    Running,   // Animating
    Paused,    // Paused mid-animation
    Completed  // Finished (ready for removal or restart)
};

/// The tween interface TweenManager stores, whatever the value type.
class DEKI_TWEEN_API ITween
{
public:
    virtual ~ITween() = default;

    virtual void Update(float deltaTimeSeconds) = 0;
    virtual bool IsComplete() const = 0;
    virtual void Kill() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual TweenState GetState() const = 0;
    virtual void Restart() = 0;

    // Unique, for lookup
    uint32_t id = 0;
};

/// A tween of one value type: float, int32_t, Deki::Vector2 or Deki::Color.
template <typename T>
class Tween : public ITween
{
public:
    using UpdateCallback = std::function<void(const T&)>;
    using CompleteCallback = std::function<void()>;

    Tween()
        : m_Target(nullptr),
          m_StartValue{},
          m_EndValue{},
          m_CurrentValue{},
          m_Duration(1.0f),
          m_Elapsed(0.0f),
          m_Delay(0.0f),
          m_DelayElapsed(0.0f),
          m_EaseFunc(Ease::Linear),
          m_State(TweenState::Idle),
          m_Loops(0),
          m_CurrentLoop(0),
          m_PingPong(false),
          m_Reversed(false)
    {
    }

    ~Tween() override = default;

    // ========== Configuration (Fluent API) ==========

    /// The value the tween writes to directly.
    Tween<T>& SetTarget(T* target)
    {
        m_Target = target;
        return *this;
    }

    Tween<T>& From(const T& startValue)
    {
        m_StartValue = startValue;
        return *this;
    }

    Tween<T>& To(const T& endValue)
    {
        m_EndValue = endValue;
        return *this;
    }

    /// Duration in seconds.
    Tween<T>& Duration(float seconds)
    {
        m_Duration = seconds > 0.0f ? seconds : 0.001f;
        return *this;
    }

    Tween<T>& SetEase(EaseType ease)
    {
        m_EaseFunc = Ease::GetFunction(ease);
        return *this;
    }

    /// A custom easing function; null means linear.
    Tween<T>& SetEase(EasingFunc easeFunc)
    {
        m_EaseFunc = easeFunc ? easeFunc : Ease::Linear;
        return *this;
    }

    /// Delay before starting, in seconds.
    Tween<T>& Delay(float seconds)
    {
        m_Delay = seconds > 0.0f ? seconds : 0.0f;
        return *this;
    }

    /// Number of loops: -1 = forever, 0 = play once.
    Tween<T>& SetLoops(int32_t loops)
    {
        m_Loops = loops;
        return *this;
    }

    /// Ping-pong: reverse direction on each loop.
    Tween<T>& SetPingPong(bool pingPong)
    {
        m_PingPong = pingPong;
        return *this;
    }

    /// Called each frame with the current value.
    Tween<T>& OnUpdate(UpdateCallback callback)
    {
        m_OnUpdate = callback;
        return *this;
    }

    /// Called once the tween finishes; Kill() does not call it.
    Tween<T>& OnComplete(CompleteCallback callback)
    {
        m_OnComplete = callback;
        return *this;
    }

    /// Starts the tween. TweenManager calls it when the tween is added.
    Tween<T>& Start()
    {
        if (m_State == TweenState::Idle || m_State == TweenState::Completed)
        {
            m_State = TweenState::Running;
            m_Elapsed = 0.0f;
            m_DelayElapsed = 0.0f;
            m_CurrentLoop = 0;
            m_Reversed = false;
            if (m_Target && m_Delay <= 0.0f)
            {
                m_CurrentValue = m_StartValue;
                *m_Target = m_CurrentValue;
            }
        }
        return *this;
    }

    // ========== Control ==========

    void Update(float deltaTimeSeconds) override
    {
        if (m_State != TweenState::Running)
        {
            return;
        }

        float dt = deltaTimeSeconds;

        if (m_DelayElapsed < m_Delay)
        {
            m_DelayElapsed += dt;
            if (m_DelayElapsed < m_Delay)
            {
                return;
            }
            // Use the time left over after the delay.
            dt = m_DelayElapsed - m_Delay;
        }

        m_Elapsed += dt;

        float progress = m_Elapsed / m_Duration;
        if (progress >= 1.0f)
        {
            progress = 1.0f;
        }

        float easedInput = m_Reversed ? (1.0f - progress) : progress;
        float easedProgress = m_EaseFunc(easedInput);

        m_CurrentValue = Interpolate(easedProgress);

        ApplyValue();

        if (progress >= 1.0f)
        {
            HandleLoopOrComplete();
        }
    }

    bool IsComplete() const override { return m_State == TweenState::Completed; }

    void Kill() override
    {
        m_State = TweenState::Completed;
        // Kill does NOT call OnComplete.
    }

    void Pause() override
    {
        if (m_State == TweenState::Running)
        {
            m_State = TweenState::Paused;
        }
    }

    void Resume() override
    {
        if (m_State == TweenState::Paused)
        {
            m_State = TweenState::Running;
        }
    }

    TweenState GetState() const override { return m_State; }

    void Restart() override
    {
        m_State = TweenState::Idle;
        Start();
    }

    const T& GetCurrentValue() const { return m_CurrentValue; }

    T* GetTarget() const { return m_Target; }

private:
    T* m_Target;  // The value being tweened (optional)
    T m_StartValue;
    T m_EndValue;
    T m_CurrentValue;

    float m_Duration;
    float m_Elapsed;
    float m_Delay;
    float m_DelayElapsed;

    EasingFunc m_EaseFunc;
    TweenState m_State;

    int32_t m_Loops;  // -1 = forever
    int32_t m_CurrentLoop;
    bool m_PingPong;  // Reverse direction each loop
    bool m_Reversed;  // Playing in reverse now

    UpdateCallback m_OnUpdate;
    CompleteCallback m_OnComplete;

    // The value between start and end at eased progress t in [0,1].
    T Interpolate(float t) const;

    // Writes the current value to the target and the update callback.
    void ApplyValue()
    {
        if (m_Target)
        {
            *m_Target = m_CurrentValue;
        }
        if (m_OnUpdate)
        {
            m_OnUpdate(m_CurrentValue);
        }
    }

    // At the end of a pass: start the next loop, or complete.
    void HandleLoopOrComplete()
    {
        if (m_Loops == -1 || m_CurrentLoop < m_Loops)
        {
            m_CurrentLoop++;
            m_Elapsed = 0.0f;

            if (m_PingPong)
            {
                m_Reversed = !m_Reversed;
            }
        }
        else
        {
            m_State = TweenState::Completed;
            if (m_OnComplete)
            {
                m_OnComplete();
            }
        }
    }
};

// ========== Interpolation for each value type ==========
// Defined in Tween.cpp.

template <>
float Tween<float>::Interpolate(float t) const;

template <>
int32_t Tween<int32_t>::Interpolate(float t) const;

template <>
Deki::Vector2 Tween<Deki::Vector2>::Interpolate(float t) const;

template <>
Deki::Color Tween<Deki::Color>::Interpolate(float t) const;

}  // namespace DekiTween
