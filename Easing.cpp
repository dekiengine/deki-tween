#include "Easing.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace deki {
namespace Ease {

float Linear(float t)
{
    return t;
}

// Sine
float SineIn(float t)
{
    return 1.0f - std::cos((t * static_cast<float>(M_PI)) / 2.0f);
}

float SineOut(float t)
{
    return std::sin((t * static_cast<float>(M_PI)) / 2.0f);
}

float SineInOut(float t)
{
    return -(std::cos(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;
}

// Quad
float QuadIn(float t)
{
    return t * t;
}

float QuadOut(float t)
{
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float QuadInOut(float t)
{
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

// Cubic
float CubicIn(float t)
{
    return t * t * t;
}

float CubicOut(float t)
{
    return 1.0f - std::pow(1.0f - t, 3.0f);
}

float CubicInOut(float t)
{
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

// Quart
float QuartIn(float t)
{
    return t * t * t * t;
}

float QuartOut(float t)
{
    return 1.0f - std::pow(1.0f - t, 4.0f);
}

float QuartInOut(float t)
{
    return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
}

// Quint
float QuintIn(float t)
{
    return t * t * t * t * t;
}

float QuintOut(float t)
{
    return 1.0f - std::pow(1.0f - t, 5.0f);
}

float QuintInOut(float t)
{
    return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
}

// Expo
float ExpoIn(float t)
{
    return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
}

float ExpoOut(float t)
{
    return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}

float ExpoInOut(float t)
{
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
                    : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

// Circ
float CircIn(float t)
{
    return 1.0f - std::sqrt(1.0f - t * t);
}

float CircOut(float t)
{
    return std::sqrt(1.0f - (t - 1.0f) * (t - 1.0f));
}

float CircInOut(float t)
{
    return t < 0.5f ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f
                    : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

// Back
static constexpr float c1 = 1.70158f;
static constexpr float c2 = c1 * 1.525f;
static constexpr float c3 = c1 + 1.0f;

float BackIn(float t)
{
    return c3 * t * t * t - c1 * t * t;
}

float BackOut(float t)
{
    return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float BackInOut(float t)
{
    return t < 0.5f ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
                    : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

// Elastic
static constexpr float c4 = (2.0f * static_cast<float>(M_PI)) / 3.0f;
static constexpr float c5 = (2.0f * static_cast<float>(M_PI)) / 4.5f;

float ElasticIn(float t)
{
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}

float ElasticOut(float t)
{
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float ElasticInOut(float t)
{
    if (t == 0.0f)
        return 0.0f;
    if (t == 1.0f)
        return 1.0f;
    return t < 0.5f ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
                    : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

// Bounce helper
static float BounceOutImpl(float t)
{
    constexpr float n1 = 7.5625f;
    constexpr float d1 = 2.75f;

    if (t < 1.0f / d1)
    {
        return n1 * t * t;
    }
    else if (t < 2.0f / d1)
    {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    }
    else if (t < 2.5f / d1)
    {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    }
    else
    {
        t -= 2.625f / d1;
        return n1 * t * t + 0.984375f;
    }
}

float BounceIn(float t)
{
    return 1.0f - BounceOutImpl(1.0f - t);
}

float BounceOut(float t)
{
    return BounceOutImpl(t);
}

float BounceInOut(float t)
{
    return t < 0.5f ? (1.0f - BounceOutImpl(1.0f - 2.0f * t)) / 2.0f
                    : (1.0f + BounceOutImpl(2.0f * t - 1.0f)) / 2.0f;
}

EasingFunc GetFunction(EaseType type)
{
    switch (type)
    {
    case EaseType::Linear:
        return Linear;
    case EaseType::SineIn:
        return SineIn;
    case EaseType::SineOut:
        return SineOut;
    case EaseType::SineInOut:
        return SineInOut;
    case EaseType::QuadIn:
        return QuadIn;
    case EaseType::QuadOut:
        return QuadOut;
    case EaseType::QuadInOut:
        return QuadInOut;
    case EaseType::CubicIn:
        return CubicIn;
    case EaseType::CubicOut:
        return CubicOut;
    case EaseType::CubicInOut:
        return CubicInOut;
    case EaseType::QuartIn:
        return QuartIn;
    case EaseType::QuartOut:
        return QuartOut;
    case EaseType::QuartInOut:
        return QuartInOut;
    case EaseType::QuintIn:
        return QuintIn;
    case EaseType::QuintOut:
        return QuintOut;
    case EaseType::QuintInOut:
        return QuintInOut;
    case EaseType::ExpoIn:
        return ExpoIn;
    case EaseType::ExpoOut:
        return ExpoOut;
    case EaseType::ExpoInOut:
        return ExpoInOut;
    case EaseType::CircIn:
        return CircIn;
    case EaseType::CircOut:
        return CircOut;
    case EaseType::CircInOut:
        return CircInOut;
    case EaseType::BackIn:
        return BackIn;
    case EaseType::BackOut:
        return BackOut;
    case EaseType::BackInOut:
        return BackInOut;
    case EaseType::ElasticIn:
        return ElasticIn;
    case EaseType::ElasticOut:
        return ElasticOut;
    case EaseType::ElasticInOut:
        return ElasticInOut;
    case EaseType::BounceIn:
        return BounceIn;
    case EaseType::BounceOut:
        return BounceOut;
    case EaseType::BounceInOut:
        return BounceInOut;
    default:
        return Linear;
    }
}

} // namespace Ease
} // namespace deki
