
#ifndef SP_HASH_OBJECT_HPP
#define SP_HASH_OBJECT_HPP

#include "Utils.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


namespace sp_hash
{
	class IObject //: NonCopyable, NonMovable
	{
	public:
		virtual ~IObject(){}
		virtual void handleEvents(sf::RenderWindow& window, sf::Event& event) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
	};

	class Object : public IObject
	{
	public:
		Object() = default;
		explicit Object(float radius, const sf::Vector2f& position = sf::Vector2f(0, 0), bool setOriginToCenter = true);
		virtual ~Object();

		/* Setters */
		void setPosition(const sf::Vector2f& position);
		void setRadius(float radius);
		void setFillColor(const sf::Color& color);
		void setLeftSelectionColor(const sf::Color& color);
		void setRightSelectionColor(const sf::Color& color);
		void setOriginToCenter();
		void setTrigger(bool trigger);

		
		/* Getters */
		bool getTrigger() const;
		const sf::Vector2f& getPosition() const;
		const sf::CircleShape& getShape() const;
		float getRadius() const;
		const sf::Color& getLeftColor() const;
		const sf::Color& getRightColor() const;

		/* Misc */
		void toggleTrigger();
		void changeSize(float size);

		virtual void handleEvents(sf::RenderWindow& window, sf::Event& event);
		virtual void update(float deltaTime);
		virtual void render(sf::RenderWindow& window);

	private:
		bool m_trigger;
		sf::Uint32 m_drawLayer;
		sf::CircleShape m_shape;
		sf::Color m_leftSelectionColor;
		sf::Color m_rightSelectionColor;

		void buildObject(float size);
	};

	inline void Object::setTrigger(bool trigger)
	{ m_trigger = trigger; }

	inline void Object::toggleTrigger()
	{ m_trigger != m_trigger; }

	inline bool Object::getTrigger() const
	{ return m_trigger; }

	inline void Object::setPosition(const sf::Vector2f& pos)
	{ m_shape.setPosition(pos); }

	inline void Object::setRadius(float radius)
	{ m_shape.setRadius(radius); }

	inline void Object::setFillColor(const sf::Color& color)
	{ m_shape.setFillColor(color); }

	inline void Object::setLeftSelectionColor(const sf::Color& color)
	{ m_leftSelectionColor = color; }

	inline void Object::setRightSelectionColor(const sf::Color& color)
	{ m_rightSelectionColor = color; }

	inline const sf::Vector2f& Object::getPosition() const
	{ return m_shape.getPosition(); }

	inline float Object::getRadius() const
	{ return m_shape.getRadius(); }

	inline const sf::CircleShape& Object::getShape() const
	{ return m_shape; }

	inline const sf::Color& Object::getLeftColor() const
	{ return m_leftSelectionColor; }

	inline const sf::Color& Object::getRightColor() const
	{ return m_rightSelectionColor; }

	inline void Object::changeSize(float size)
	{ m_shape.setRadius(m_shape.getRadius() + size); }

	std::ostream& operator<<(std::ostream& os, Object& o);
}

#endif // SP_HASH_OBJECT_HPP