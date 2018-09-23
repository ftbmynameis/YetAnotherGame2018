#pragma once

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

float rad2deg(float radians)
{
	return radians * (180.0f / 3.141592653589793238463f);
}