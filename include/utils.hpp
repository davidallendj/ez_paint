#pragma once

#include "types.hpp"
#include "format.hpp"

#include <iomanip>
#include <iostream>
#include <functional>
#include <iterator>
#include <chrono>
#include <type_traits>

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <fmt/format.h>
#include <fmt/chrono.h>

namespace std
{
	template <typename StlContainer, typename Function>
	void sort(StlContainer&& c, Function&& f = []{})
	{ std::sort(c.begin(), c.end(), f); }
}

namespace fu::utils
{
	/*
	 * Useful Utility functions
	 */
	sf::View calcView(const sf::Vector2f& windowSize, float minRatio, float maxRatio);
	void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, float zoom);

	/*
	 * Operator overloads for SFML objects.
	 */
	
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v)
	{ return os << to_string(v); }

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const sf::Vector3<T>& v)
	{ return os << to_string(v); }

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r)
	{ return os << to_string(r); }

	template <typename T> using ptr = typename std::shared_ptr<T>;
	template <typename T> using shared = typename std::shared_ptr<T>;
	template <typename T> using unique = typename std::unique_ptr<T>;
	template <typename T> using vector = typename std::vector<T>;

	template <class T, class...Args>
	std::shared_ptr<T> make(Args&&...args){
		return std::shared_ptr<T>{ new T{std::forward<Args>(args)...}/*, custom_deleter<T>{} */};
	}

	std::ostream& operator<<(std::ostream& os, const sf::Color& color);

#if __cplusplus > 201703L   // for fold expressions and more constexpr
	static inline auto timestamp(const std::string& format = ":%I:%M:%S %p; %m/%d/%Y"){
		time_t t = std::time(nullptr);
		return fmt::format("{"+format+"}", fmt::localtime(t));
		// return fmt::format(format, std::chrono::system_clock::now());
	}
	
	template <typename T, typename...Args>
	void push_back(std::vector<T>& v, Args&&...args)
	{ 
		static_assert((std::is_constructible_v<T, Args&&>&& ...));
		(v.emplace_back(std::forward<Args>(args)), ...);
	}

	// A make_tuple wrapper for enforcing certain requirements
	template <typename... Args>
	auto range(Args...args)
	{
		// Limit number of args to only 2
		static_assert(sizeof...(Args) != 1, "Ranges requires only 2 arguments");
		return std::make_tuple(std::forward<Args>(args)...); 
	}

	template <typename T>
	auto min(const types::range_t<T>& values)
	{ return std::get<0>(values); }

	template <typename T>
	auto max(const types::range_t<T>& values)
	{ return std::get<1>(values); }

#ifndef FU_STL_FNDEF
	#define FU_STL_FNDEF \
		std::forward<StlContainer>(c).begin(), \
		std::forward<StlContainer>(c).end(), \
		std::forward<Function>(f)
#endif

#ifndef FU_STL_TPDEF
	#define FU_STL_TPDEF        \
		typename StlContainer,  \
		typename Function = std::function<void()>
#endif
	template < FU_STL_TPDEF >
	void foreach(StlContainer&& c, Function&& f)
	{  std::for_each(FU_STL_FNDEF); }

	template < FU_STL_TPDEF >
	auto findif(StlContainer&& c, Function&& f)
	{ return std::find_if(FU_STL_FNDEF); }

	template < typename StlContainer >
	void erase(StlContainer&& c)
	{ std::forward<StlContainer>(c).erase(c.begin(), c.end()); }

	template < typename StlContainer >
	void remove(StlContainer&& c)
	{ std::forward<StlContainer>(c).remove(c.begin(), c.end()); }

	template <typename StlContainer>
	auto erase_and_remove(StlContainer&& c, size_t index)
	{ c.erase(std::remove(c.begin(), c.end(), index)); }

	template <typename StlContainer, typename Function>
	void sort(StlContainer&& c, Function&& f = []{})
	{ std::sort(c.begin(), c.end(), f); }

	template < 
		typename StlContainer_Src, 
		typename StlContainer_Dest 
	>
	void insert_move(StlContainer_Src && src, StlContainer_Dest&& dest)
	{ 
		src.insert(src.end(), 
			std::make_move_iterator(dest.begin()),
			std::make_move_iterator(dest.end())
		);
	}

	// template <typename T>
	struct NonCopyable{
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	// template <typename T>
	struct NonMovable{
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;
	};


#ifndef PaletteColors
	#define PaletteColors { 				\
		sf::Color(0, 0, 0, 255), 			\
		sf::Color(102, 102, 102, 255), 		\
		sf::Color(255, 255, 255, 255), 		\
		sf::Color(128, 0, 0, 255), 			\
		sf::Color(255, 0, 0, 255), 			\
		sf::Color(255, 102, 0, 255), 		\
		sf::Color(255, 255, 0, 255), 		\
		sf::Color(0, 255, 0, 255), 			\
		sf::Color(0, 255, 255, 255), 		\
		sf::Color(0, 155, 255, 255), 		\
		sf::Color(0, 0, 255, 255), 			\
		sf::Color(128, 0, 255, 255), 		\
		sf::Color(191, 0, 255, 255), 		\
		sf::Color(255, 0, 255), 			\
		sf::Color(255, 0, 191, 255) 		\
	}
#endif

}
#endif // __cpluscplus