
#include "brush.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "log.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>


namespace ez_paint
{
	Brush::Brush(float size, const sf::Vector2f& position, bool setOriginToCenter)
	: m_shape(utils::make<sf::CircleShape>(size))
	, m_primaryColor(sf::Color(0, 255, 0, 100))
	, m_secondaryColor(sf::Color(255, 0, 0, 100))
	, m_strength(0.0f)
	{ 
		build(size);
		setPosition(position);
		if (setOriginToCenter)
			this->setOriginToCenter(); 
	}

	Brush::~Brush(){}
	
	void Brush::handleEvents(sf::RenderWindow& window, sf::Event& event)
	{
		using namespace utils;
		using namespace log;

		// TODO: Set to keyboard modifiers bit flags
		const bool isCtrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
		const bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
		const bool isAltPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);

		switch(event.type)
		{
			case sf::Event::Resized: 
				//window.setView(calcView(WINDOW_SIZE, 1.f, 1.f));
				break;
			case sf::Event::MouseWheelScrolled:
			{
				// float r = getRadius();
				float r = getScale();
				float rmin = min(config::BrushSize);
				float rmax = max(config::BrushSize);
				float rdelta = config::BrushSizeDelta;
				// Mouse wheel up
				if (event.mouseWheelScroll.delta > 0){
					if(!isCtrlPressed){
						// m_shape.setRadius(std::clamp(r+rdelta, rmin, rmax));
						float scale = std::clamp(r+rdelta, rmin, rmax);
						m_shape->setScale(sf::Vector2f(scale, scale));
					}
				}
				// Mouse wheel down
				else if (event.mouseWheelScroll.delta < 0){
					if(!isCtrlPressed){
						// m_shape.setRadius(std::clamp(r-rdelta, rmin, rmax));
						float scale = std::clamp(r-rdelta, rmin, rmax);
						m_shape->setScale(sf::Vector2(scale, scale));
					}
				}
				
				/* Update the brush's origin after resize */
				setOriginToCenter();
				log::info("brush.size: {}", r);
			}
			case sf::Event::MouseMoved:
				setPosition(config::MousePosition);
				break;
			
			case sf::Event::MouseButtonPressed:
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
					;
				break;
			case sf::Event::MouseButtonReleased:
				break;
		}
	}

	void Brush::update(sf::RenderWindow& window, float deltaTime)
	{
		
	}

	void Brush::render(sf::RenderWindow& window)
	{
		window.draw(*m_shape);
	}

	void Brush::setOriginToCenter()
	{
		// m_shape.setOrigin(
		// 	(m_shape.getLocalBounds().left + m_shape.getLocalBounds().width) / 2.0f,
		// 	(m_shape.getLocalBounds().top + m_shape.getLocalBounds().height) / 2.0f);
		m_shape->setOrigin(
			(m_shape->getLocalBounds().left + m_shape->getLocalBounds().width) / 2.0f,
			(m_shape->getLocalBounds().top + m_shape->getLocalBounds().height) / 2.0f
		);
	}

	void Brush::build(float size)
	{
		// m_shape.setRadius(size);
		// m_shape.setFillColor(sf::Color(0, 0, 255, 100));
		m_shape = utils::make<sf::CircleShape>();
		m_shape->setScale(sf::Vector2f(size, size));
		m_shape->setFillColor(sf::Color(0, 0, 255, 100));
	}
}