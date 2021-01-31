
#ifndef SP_HASH_BRUSH_HPP
#define SP_HASH_BRUSH_HPP

#include "Object.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

namespace sp_hash
{
    class Brush : public Object
    {
    public:
        Brush(float radius = 20.0, const sf::Vector2f& position = sf::Vector2f(0, 0), bool setOriginToCenter = true);
        virtual ~Brush();

        void setPrimaryColor(const sf::Color& color);
        void setSecondaryColor(const sf::Color& color);

        /* Setters */
		void setPosition(const sf::Vector2f& position);
		void setRadius(float radius);
		void setOriginToCenter();
		void setTrigger(bool trigger);

        /* Getters */
		const sf::Vector2f& getPosition() const;
		const sf::CircleShape& getShape() const;
		float getRadius() const;
		const sf::Color& getPrimaryColor() const;
		const sf::Color& getSecondaryColor() const;
		sf::FloatRect getBounds() const;

        /* Misc */
		void changeSize(float size);

        void handleEvents(sf::RenderWindow& window, sf::Event& event);
        void update(float deltaTime);
        void render(sf::RenderWindow& window);
    
    private:
        sf::CircleShape m_shape;        //TODO: Maybe make this sf::Shape*
		// std::unique_ptr<sf::Shape> m_shape;
		sf::Color m_primaryColor;
        sf::Color m_secondaryColor;

        void build(float size);
    };

    inline void Brush::setPrimaryColor(const sf::Color& color)
    { m_primaryColor = color; }

    inline void Brush::setSecondaryColor(const sf::Color& color)
    { m_secondaryColor = color; }

	inline void Brush::setPosition(const sf::Vector2f& pos)
	{ m_shape.setPosition(pos); }

	inline void Brush::setRadius(float radius)
	{ m_shape.setRadius(radius); }

	inline const sf::Vector2f& Brush::getPosition() const
	{ return m_shape.getPosition(); }

	inline float Brush::getRadius() const
	{ return m_shape.getRadius(); }

	inline const sf::CircleShape& Brush::getShape() const
	{ return m_shape; }

	inline sf::FloatRect Brush::getBounds() const
	{ return m_shape.getGlobalBounds(); }

	inline void Brush::changeSize(float size)
	{ m_shape.setRadius(m_shape.getRadius() + size); }

	inline const sf::Color& Brush::getPrimaryColor() const
	{ return m_primaryColor; }

	inline const sf::Color& Brush::getSecondaryColor() const
	{ return m_secondaryColor; }
}

#endif // SP_HASH_BRUSH_HPP