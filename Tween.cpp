#include "Tween.h"

namespace Deki {

// ========== Interpolation Specializations ==========
// t is normalized progress in [0,1].

template <>
float Tween<float>::Interpolate(float t) const
{
    return m_StartValue + (m_EndValue - m_StartValue) * t;
}

template <>
int32_t Tween<int32_t>::Interpolate(float t) const
{
    return static_cast<int32_t>(static_cast<float>(m_StartValue) +
                                 (static_cast<float>(m_EndValue) - static_cast<float>(m_StartValue)) * t + 0.5f);
}

template <>
Deki::Vector2 Tween<Deki::Vector2>::Interpolate(float t) const
{
    return Deki::Vector2::Lerp(m_StartValue, m_EndValue, t);
}

template <>
Deki::Color Tween<Deki::Color>::Interpolate(float t) const
{
    return m_StartValue.Lerp(m_EndValue, t);
}

} // namespace Deki
