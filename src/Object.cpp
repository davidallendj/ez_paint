
#include "Object.hpp"

namespace sp_hash
{
	Object::Object(float radius, const sf::Vector2f& position, bool setOriginToCenter) 
	: m_trigger(false)
	, m_drawLayer(0)
	, m_shape(sf::CircleShape())
	, m_leftSelectionColor(sf::Color::Blue)
	, m_rightSelectionColor(sf::Color::Red)
	{ 
		buildObject(radius);
		setPosition(position);
		if (setOriginToCenter)
			this->setOriginToCenter();
	}

	Object::~Object(){}

	void Object::setOriginToCenter()
	{
		m_shape.setOrigin(
			(m_shape.getLocalBounds().left + m_shape.getLocalBounds().width) / 2.0f,
			(m_shape.getLocalBounds().top + m_shape.getLocalBounds().height) / 2.0f);
	}

	void Object::buildObject(float size)
	{
		m_shape.setRadius(size);
		m_shape.setFillColor(sf::Color(0, 0, 255, 100));
		m_leftSelectionColor = sf::Color::Cyan;
		m_rightSelectionColor = sf::Color::White;
	}

	void Object::handleEvents(sf::RenderWindow& window, sf::Event& event){}
	void Object::update(float deltaTime){}
	void Object::render(sf::RenderWindow& window){}

	std::ostream& operator<<(std::ostream& os, Object& o)
    { return os << ""; }
}
