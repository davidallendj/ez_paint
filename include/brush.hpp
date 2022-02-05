
#ifndef FU_BRUSH_HPP
#define FU_BRUSH_HPP

#include "object.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

// export module my_test;

namespace fu
{
	class Brush : public Object
	{
	public:
		typedef std::shared_ptr<sf::Shape> Shape;

		Brush(float size = 20.0f, const sf::Vector2f& position = sf::Vector2f(0, 0), bool setOriginToCenter = true);
		virtual ~Brush();

		void setPrimaryColor(const sf::Color& color);
		void setSecondaryColor(const sf::Color& color);

		/* Setters */
		void setPosition(const sf::Vector2f& position);
		// void setRadius(float radius);
		void setScale(float scale);
		void setOriginToCenter();

		/* Getters */
		const sf::Vector2f& getPosition() const;
		Shape getShape() const;
		// float getRadius() const;
		float getScale() const;
		const sf::Color& getPrimaryColor() const;
		const sf::Color& getSecondaryColor() const;
		sf::FloatRect getBounds() const;

		/* Misc */
		void changeSize(float size);

		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(sf::RenderWindow& window, float deltaTime);
		void render(sf::RenderWindow& window);
	
	private:
		// sf::CircleShape m_shape;        //TODO: Maybe make this sf::Shape*
		Shape m_shape;
		sf::Color m_primaryColor;
		sf::Color m_secondaryColor;
		float m_strength;

		void build(float size);
	};

	inline void Brush::setPrimaryColor(const sf::Color& color)
	{ m_primaryColor = color; }

	inline void Brush::setSecondaryColor(const sf::Color& color)
	{ m_secondaryColor = color; }

	inline void Brush::setPosition(const sf::Vector2f& pos)
	{ m_shape->setPosition(pos); }

	inline void Brush::setScale(float scale)
	{ m_shape->setScale(sf::Vector2f(scale, scale)); }

	// inline void Brush::setRadius(float radius)
	// { m_shape->setRadius(radius); }

	inline const sf::Vector2f& Brush::getPosition() const
	{ return m_shape->getPosition(); }

	inline float Brush::getScale() const
	{ return m_shape->getScale().x; }

	// inline float Brush::getRadius() const
	// { return m_shape->getRadius(); }

	inline Brush::Shape Brush::getShape() const
	{ return m_shape; }

	inline sf::FloatRect Brush::getBounds() const
	{ return m_shape->getGlobalBounds(); }

	inline void Brush::changeSize(float size)
	{
		sf::Vector2f scale = m_shape->getScale();
		m_shape->setScale(scale + sf::Vector2f(size, size)); 
		// m_shape->setRadius(m_shape->getRadius() + size); 
	}

	inline const sf::Color& Brush::getPrimaryColor() const
	{ return m_primaryColor; }

	inline const sf::Color& Brush::getSecondaryColor() const
	{ return m_secondaryColor; }
}

#endif // FU_BRUSH_HPP