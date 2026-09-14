// Properties every easing curve has to hold, checked across all of them.
//
// There are 31 curves and they are all small closed-form expressions, which is
// exactly the shape where a transposed constant or a sign slip compiles, looks
// plausible in isolation, and animates wrongly. Testing each curve's shape by
// hand would mean 31 sets of hand-computed values; testing the properties they
// all share catches the same mistakes and keeps working when a curve is added.

#include <gtest/gtest.h>

#include <Easing.h>

#include <cmath>
#include <vector>

namespace
{

using Deki::EaseType;

std::vector<EaseType> AllTypes()
{
    std::vector<EaseType> all;
    for (uint8_t i = 0; i < static_cast<uint8_t>(EaseType::COUNT); ++i)
        all.push_back(static_cast<EaseType>(i));
    return all;
}

// Samples across the unit interval, endpoints included.
std::vector<float> Samples(int n = 101)
{
    std::vector<float> ts;
    for (int i = 0; i < n; ++i)
        ts.push_back(static_cast<float>(i) / static_cast<float>(n - 1));
    return ts;
}

}  // namespace

TEST(Easing, EveryTypeResolvesToAFunction)
{
    // A type added to the enum but forgotten in GetFunction's switch returns
    // null and crashes at the call site instead of here.
    for (EaseType t : AllTypes())
        EXPECT_NE(Deki::Ease::GetFunction(t), nullptr)
            << "no function for ease type " << static_cast<int>(t);
}

TEST(Easing, EveryCurveStartsAtZeroAndEndsAtOne)
{
    // The contract of an easing function: it maps the unit interval onto
    // itself at the endpoints. Curves that overshoot in between (Back,
    // Elastic) still have to land exactly.
    for (EaseType t : AllTypes())
    {
        auto f = Deki::Ease::GetFunction(t);
        ASSERT_NE(f, nullptr);
        EXPECT_NEAR(f(0.0f), 0.0f, 1e-5f) << "ease type " << static_cast<int>(t) << " at t=0";
        EXPECT_NEAR(f(1.0f), 1.0f, 1e-5f) << "ease type " << static_cast<int>(t) << " at t=1";
    }
}

TEST(Easing, EveryCurveIsFiniteAcrossTheInterval)
{
    // A division by zero or a pow with a negative base shows up as inf or nan
    // rather than as a wrong number, and reaches a transform unnoticed.
    for (EaseType t : AllTypes())
    {
        auto f = Deki::Ease::GetFunction(t);
        ASSERT_NE(f, nullptr);
        for (float x : Samples())
            EXPECT_TRUE(std::isfinite(f(x)))
                << "ease type " << static_cast<int>(t) << " is not finite at t=" << x;
    }
}

TEST(Easing, NonOvershootingCurvesStayInRange)
{
    // Back and Elastic overshoot by design; every other curve staying inside
    // [0,1] is what lets a caller use the result as a blend factor directly.
    for (EaseType t : AllTypes())
    {
        const int i = static_cast<int>(t);
        const bool overshoots =
            (t == EaseType::BackIn || t == EaseType::BackOut || t == EaseType::BackInOut ||
             t == EaseType::ElasticIn || t == EaseType::ElasticOut || t == EaseType::ElasticInOut);
        if (overshoots)
            continue;

        auto f = Deki::Ease::GetFunction(t);
        ASSERT_NE(f, nullptr);
        for (float x : Samples())
        {
            const float y = f(x);
            EXPECT_GE(y, -1e-5f) << "ease type " << i << " dips below 0 at t=" << x;
            EXPECT_LE(y, 1.0f + 1e-5f) << "ease type " << i << " exceeds 1 at t=" << x;
        }
    }
}

TEST(Easing, InAndOutAreMirrorImages)
{
    // out(t) == 1 - in(1 - t) is the definition of the Out variant, and the
    // place a copy-paste between two families goes wrong.
    struct Pair
    {
        EaseType in;
        EaseType out;
    };
    const Pair pairs[] = {
        {EaseType::SineIn, EaseType::SineOut},   {EaseType::QuadIn, EaseType::QuadOut},
        {EaseType::CubicIn, EaseType::CubicOut}, {EaseType::QuartIn, EaseType::QuartOut},
        {EaseType::QuintIn, EaseType::QuintOut}, {EaseType::ExpoIn, EaseType::ExpoOut},
        {EaseType::CircIn, EaseType::CircOut},   {EaseType::BackIn, EaseType::BackOut},
        {EaseType::BounceIn, EaseType::BounceOut},
    };
    for (const Pair& p : pairs)
    {
        auto in = Deki::Ease::GetFunction(p.in);
        auto out = Deki::Ease::GetFunction(p.out);
        ASSERT_NE(in, nullptr);
        ASSERT_NE(out, nullptr);
        for (float x : Samples(21))
            EXPECT_NEAR(out(x), 1.0f - in(1.0f - x), 1e-4f)
                << "ease types " << static_cast<int>(p.in) << "/" << static_cast<int>(p.out)
                << " are not mirrored at t=" << x;
    }
}

TEST(Easing, InOutCurvesAreSymmetricAboutTheMidpoint)
{
    // f(t) + f(1-t) == 1 for an InOut curve. Bounce is excluded: its InOut is
    // built from the piecewise Bounce halves and is symmetric only to within
    // the piece boundaries.
    const EaseType inOuts[] = {
        EaseType::SineInOut,  EaseType::QuadInOut, EaseType::CubicInOut, EaseType::QuartInOut,
        EaseType::QuintInOut, EaseType::ExpoInOut, EaseType::CircInOut,
    };
    for (EaseType t : inOuts)
    {
        auto f = Deki::Ease::GetFunction(t);
        ASSERT_NE(f, nullptr);
        EXPECT_NEAR(f(0.5f), 0.5f, 1e-4f) << "ease type " << static_cast<int>(t) << " midpoint";
        for (float x : Samples(21))
            EXPECT_NEAR(f(x) + f(1.0f - x), 1.0f, 1e-4f)
                << "ease type " << static_cast<int>(t) << " asymmetric at t=" << x;
    }
}

TEST(Easing, MonotonicCurvesNeverGoBackwards)
{
    // An eased value that decreases while t increases reads as a stutter.
    // Overshooting and bouncing curves are meant to reverse; the rest are not.
    for (EaseType t : AllTypes())
    {
        const bool mayReverse =
            (t == EaseType::BackIn || t == EaseType::BackOut || t == EaseType::BackInOut ||
             t == EaseType::ElasticIn || t == EaseType::ElasticOut ||
             t == EaseType::ElasticInOut || t == EaseType::BounceIn || t == EaseType::BounceOut ||
             t == EaseType::BounceInOut);
        if (mayReverse)
            continue;

        auto f = Deki::Ease::GetFunction(t);
        ASSERT_NE(f, nullptr);
        float previous = f(0.0f);
        for (float x : Samples())
        {
            const float y = f(x);
            EXPECT_GE(y, previous - 1e-5f)
                << "ease type " << static_cast<int>(t) << " decreases at t=" << x;
            previous = y;
        }
    }
}

TEST(Easing, LinearIsTheIdentity)
{
    auto f = Deki::Ease::GetFunction(EaseType::Linear);
    ASSERT_NE(f, nullptr);
    for (float x : Samples(21))
        EXPECT_FLOAT_EQ(f(x), x);
}
