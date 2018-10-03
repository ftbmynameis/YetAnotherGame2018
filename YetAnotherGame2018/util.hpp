#pragma once

#include <type_traits>

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

float rad2deg(float radians);

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

template <typename T>
T lerp(const T& a, const T& b, float alpha)
{
	return (b - a) * alpha;
}