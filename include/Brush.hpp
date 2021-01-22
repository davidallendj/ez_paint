
#ifndef SP_HASH_BRUSH_HPP
#define SP_HASH_BRUSH_HPP

#include "Object.hpp"

namespace sp_hash
{
    class Brush : public Object
    {
    public:
        Brush(float radius = 24.0, const sf::Vector2f& position = sf::Vector2f(0, 0), bool setOriginToCenter = true);
        virtual ~Brush(){}

        void handleEvents(sf::RenderWindow& window, sf::Event& event);
        void update(float deltaTime);
        void render(sf::RenderWindow& window);
    };
}

#endif // SP_HASH_BRUSH_HPP