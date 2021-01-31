#ifndef SP_HASH_HPP
#define SP_HASH_HPP

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <functional>
#include <iterator>

#include "imgui.h"
#include "imgui-SFML.h"

namespace sp_hash
{
    sf::View calcView(const sf::Vector2f& windowSize, float minRatio, float maxRatio);
    void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, float zoom);


#if __cplusplus > 201703L
    // Using C++17 fold expressions
    template <class...Args>
    constexpr void print(Args...args)
    { std::cout << ... << "\n"; }
#else
    template <class...Format>
    constexpr void print(Format&&...args)
    { print(std::forward<Format>(args)...); }

    template <class Format = sf::String>
    constexpr void printDebug(Format arg)
    { std::cout << "debug: " << arg; }
#endif

    template <
        typename StlContainer, 
        typename Function = std::function<void()>
    >
    void foreach(StlContainer&& c, Function&& f)
    { std::for_each(std::forward<StlContainer>(c).begin(), std::forward<StlContainer>(c).end(), std::forward<Function>(f)); 
    }

    template < typename StlContainer >
    void erase(StlContainer&& c)
    { std::forward<StlContainer>(c).erase(c.begin(), c.end()); }

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
    
    typedef sf::RenderWindow RenderWindow;
    typedef sf::Event Event;
    typedef sf::Clock Clock;
    typedef sf::Time Time;

    // Overload operator to print formated sf::Vector2<T>
    template <typename T>
    std::ostream& operator<<(std::ostream& os, sf::Vector2<T>& v)
    { return os << "(" << v.x << ", " << v.y << ")"; }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r)
    { return os << "(" << r.left << ", " << r.top << ", " << r.width << ", " << r.height << ")"; }

    std::ostream& operator<<(std::ostream& os, sf::Color& color);
}



#endif