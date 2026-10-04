#pragma once

#include <cstdint>
#include <functional>
#include <deki/Component.h>
#include <deki/reflection/Property.h>
#include "Easing.h"
#include <deki/Vector.h>
#include <deki/Color.h>

namespace DekiTween
{

/// Which property a TweenComponent animates, and which parts of its
/// Deki::Vector3 endValue it uses:
/// - Position: X,Y
/// - Scale: X,Y
/// - Rotation: Z (radians, as everywhere in the engine)
enum class TweenTargetType : uint8_t
{
    Position = 0,  // X,Y position
    Scale,         // X,Y scale
    Rotation,      // rotation, from the Z component
    Count
};

/// A tween set up in the inspector, no code needed. Animates the object's
/// position, scale or rotation from where it is to endValue (see
/// TweenTargetType for the parts used), with duration, delay, easing,
/// looping, ping-pong and auto-play. Completion callbacks let tweens chain or
/// trigger other behaviour.
DEKI_CATEGORY("Animation")
DEKI_DESCRIPTION("Animates the object's position, scale or rotation along an easing curve.")
DEKI_FORMER_NAME("TweenComponent")
class DEKI_TWEEN_API TweenComponent : public Deki::Component
{
public:
    // ========== Inspector Properties ==========

    DEKI_EXPORT
    DEKI_TOOLTIP("Which property of the object is animated: position, scale, rotation or colour.")
    TweenTargetType targetType = TweenTargetType::Position;

    /// Position and Scale use X,Y; Rotation uses Z.
    DEKI_EXPORT
    DEKI_TOOLTIP("The value to arrive at. Which parts are used depends on the target above.")
    Deki::Vector3 endValue = Deki::Vector3(0.0f, 0.0f, 0.0f);

    DEKI_EXPORT
    DEKI_TOOLTIP("How long one pass takes, in seconds.")
    DEKI_UNIT(Time)
    DEKI_SLIDER(0.1f, 10.0f)
    float duration = 1.0f;

    DEKI_EXPORT
    DEKI_TOOLTIP("Wait this long, in seconds, before starting.")
    DEKI_UNIT(Time)
    DEKI_SLIDER(0.0f, 5.0f)
    float delay = 0.0f;

    DEKI_EXPORT
    DEKI_TOOLTIP("The shape of the motion. Linear is mechanical; ease-in-out starts and ends gently, which reads as "
                 "natural for almost everything.")
    DekiTween::EaseType easeType = DekiTween::EaseType::Linear;

    DEKI_EXPORT
    DEKI_TOOLTIP("How many times to run. 0 runs once, -1 repeats forever.")
    DEKI_RANGE(-1, 100)
    int32_t loops = 0;

    DEKI_EXPORT
    DEKI_TOOLTIP("Run the tween backwards on alternate passes instead of jumping back to the start.")
    bool pingPong = false;

    /// Plays from Start().
    DEKI_EXPORT
    DEKI_TOOLTIP("Start as soon as the object comes alive. Off, something has to start it.")
    bool autoPlay = true;

    DEKI_EXPORT
    DEKI_TOOLTIP(
        "Treat the end value as an offset from where the object already is, rather than an absolute destination.")
    bool relative = false;

    // ========== Runtime Callbacks ==========

    std::function<void()> onComplete;
    std::function<void(float progress)> onUpdate;

    // ========== Lifecycle ==========

    TweenComponent();
    virtual ~TweenComponent();

    void Awake() override;
    void Start() override;
    void Update() override;

    // ========== Control API ==========

    /// Starts or restarts the tween.
    void Play();

    void Pause();

    void Resume();

    /// Stops and resets the tween.
    void Stop();

    void SetOnComplete(std::function<void()> callback);

    bool IsPlaying() const { return m_IsPlaying; }

    bool HasCompleted() const { return m_HasCompleted; }

    /// Progress of the current pass, 0 to 1.
    float GetProgress() const;

private:
    float m_Elapsed = 0.0f;
    float m_DelayElapsed = 0.0f;
    int32_t m_CurrentLoop = 0;
    bool m_Reversed = false;
    bool m_HasCompleted = false;
    bool m_IsPlaying = false;
    bool m_IsPaused = false;

    // The property's value when the tween started
    Deki::Vector3 m_StartValue = Deki::Vector3(0.0f, 0.0f, 0.0f);

    // The target property's current value as a Deki::Vector3: position or
    // scale in X,Y, rotation in Z.
    Deki::Vector3 GetCurrentValue() const;

    // Writes the value at eased progress `easedT` to the target property.
    void ApplyValue(float easedT);

    float GetEasedProgress(float t) const;

    // At the end of a pass: start the next loop, or complete.
    void HandleLoopOrComplete();
};

}  // namespace DekiTween
