#pragma once

// Central header of the Deki Tween package: values animated along easing
// curves.
//
// Two ways to use it:
//
// 1. The static API, from code:
//
//      #include "TweenPackage.h"
//
//      // A float
//      DekiTween::TweenManager::To(&myValue, 100.0f, 0.5f)
//          .SetEase(DekiTween::EaseType::QuadOut)
//          .OnComplete([]() { });
//
//      // A Deki::Vector2
//      DekiTween::TweenManager::To(&position, Deki::Vector2(100, 200), 1.0f)
//          .SetEase(DekiTween::EaseType::SineInOut);
//
//      // A callback after a delay
//      DekiTween::TweenManager::DelayedCall(2.0f, []() { });
//
// 2. TweenComponent, set up in the editor: add it to a Deki::Object and pick
//    the target, end value, duration, easing and so on.
//
// Easing types:
// - Linear
// - Sine: SineIn, SineOut, SineInOut
// - Quad: QuadIn, QuadOut, QuadInOut
// - Cubic: CubicIn, CubicOut, CubicInOut
// - Quart: QuartIn, QuartOut, QuartInOut
// - Quint: QuintIn, QuintOut, QuintInOut
// - Expo: ExpoIn, ExpoOut, ExpoInOut
// - Circ: CircIn, CircOut, CircInOut
// - Back: BackIn, BackOut, BackInOut
// - Elastic: ElasticIn, ElasticOut, ElasticInOut
// - Bounce: BounceIn, BounceOut, BounceInOut

#ifdef DEKI_EDITOR
#ifdef _WIN32
#ifdef DEKI_TWEEN_EXPORTS
#define DEKI_TWEEN_API __declspec(dllexport)
#else
#define DEKI_TWEEN_API __declspec(dllimport)
#endif
#else
#define DEKI_TWEEN_API
#endif
#else
#define DEKI_TWEEN_API
#endif

// Every package header, when the package is enabled
#ifdef DEKI_PACKAGE_TWEEN

#include "Easing.h"
#include "Tween.h"
#include "TweenManager.h"
#include "TweenComponent.h"

#endif  // DEKI_PACKAGE_TWEEN
