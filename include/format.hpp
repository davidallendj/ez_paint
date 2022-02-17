
#pragma once

#include "types.hpp"

#include <type_traits>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <fmt/format.h>

namespace ez_paint::format
{
	using namespace types;
	using namespace std::string_literals;

#ifndef fu_type_errors
#define fu_type_errors
#define fu_numeric_type_error "Requires numeric type"
#define fu_integral_type_error "Requires integral type"
#endif

#if __cplusplus > 201703L   // for fold expressions and more constexpr
	// static constexpr auto fu_error_message = "Requires numeric type"s;
	
	template <Numeric T>
	static std::string to_string(const sf::Rect<T>& r){
		static_assert(Numeric<T>, fu_numeric_type_error);
		return fmt::format("rect({}, {}, {}, {})", 
			r.top, r.left, r.width, r.height
		);
	}

	template <typename T> requires Numeric<T>
	static std::string to_string(const sf::Vector2<T>& v){
		static_assert(std::is_arithmetic_v<T>, fu_numeric_type_error);
		return fmt::format("vec2({}, {})", v.x, v.y);
	}

	template <typename T>
	static std::string to_string(const sf::Vector3<T>& v){
		static_assert(std::is_arithmetic_v<T>, fu_numeric_type_error);
		return fmt::format("vec3({}, {}, {})", v.x, v.y, v.z);
	}

	static inline std::string to_string(const sf::Color& c){
		return fmt::format("color({}, {}, {}, {})", c.r, c.g, c.b, c.a);
	}

	template <typename T>
	static std::string to_string(const std::tuple<T, T>& t){
		static_assert(std::is_arithmetic_v<T>, fu_integral_type_error);
		return fmt::format("range({}, {})", std::get<0>(t), std::get<1>(t));
	}
	
	constexpr static const char* to_string(bool b)
	{ return (b) ? "true" : "false"; }

#endif // __cpluscplus
}
