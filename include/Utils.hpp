#ifndef SP_HASH_HPP
#define SP_HASH_HPP

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <functional>

namespace sp_hash
{
    sf::View calcView(const sf::Vector2f& windowSize, float minRatio, float maxRatio);
    void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, float zoom);

    template <class...Format>
    constexpr void printDebug(Format&&...args)
    { printDebug(std::forward<Format>(args)...); }

    // template <class Format = sf::String>
    // constexpr void printDebug(Format arg)
    // { std::cout << "debug: " << arg; }

#if __cplusplus > 201703L
    // Using C++17 fold expressions
    template <class...Args>
    constexpr void print(Args...args)
    { std::cout << ... << "\n"; }
#endif

    template <
        typename StlContainer, 
        typename Function = std::function<void()>
    >
    void foreach(StlContainer&& c, Function&& f)
    { std::for_each(std::forward<StlContainer>(c).begin(), std::forward<StlContainer>(c).end(), std::forward<Function>(f)); 
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
    
    using RenderWindow = sf::RenderWindow;
    using Event = sf::Event;
    using Clock = sf::Clock;
    using Time = sf::Time;
}

namespace sf
{
    // Overload operator to print formated sf::Vector2<T>
    template <typename T>
    std::ostream& operator<<(std::ostream& os, Vector2<T>& v)
    { return os << "(" << v.x << ", " << v.y << ")"; }
}

#endif