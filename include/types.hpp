#pragma once

#include "canvas_layer.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <memory>
#include <vector>
#include <type_traits>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace fu::types
{
	template <typename T> concept Numeric = std::is_arithmetic_v<T>;
	template <typename T> concept Integral = std::is_integral_v<T>;
	template <typename T> concept Abstract = std::is_abstract_v<T>;
	template <typename T> concept Summable = requires(T a, T b){ a + b; };
	template <typename T> concept Shape = std::is_base_of_v<sf::Shape, T>;
	template <typename T> concept Pointer = std::is_pointer_v<T>;

	template <typename T> using ptr_type = std::shared_ptr<T>;
	template <typename T> using range_t = std::tuple<T, T>;
	using rangef_t = range_t<float>;
	using rangei_t = range_t<int>;
	using rangeu_t = range_t<unsigned int>;
	using rangel_t = range_t<long>;
	using rangeul_t = range_t<unsigned long>;


	typedef sf::RenderWindow RenderWindow;
	typedef sf::Event Event;
	typedef sf::Clock Clock;
	typedef sf::Time Time;
	typedef std::vector<sf::Color> Palette;
	typedef std::vector<CanvasLayer> CanvasLayers;
}