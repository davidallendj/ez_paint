
#include "Brush.hpp"
#include "AppConfig.hpp"
#include "Utils.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sp_hash
{
    Brush::Brush(float radius, const sf::Vector2f& position, bool setOriginToCenter)
    : m_primaryColor(sf::Color::Black)
    , m_secondaryColor(sf::Color::White)
    { 
        build(radius);
		setPosition(position);
		if (setOriginToCenter)
			this->setOriginToCenter(); 
    }

    Brush::~Brush(){}

    void Brush::handleEvents(sf::RenderWindow& window, sf::Event& event)
    {
        bool isCtrlPressed = false;
        switch(event.type)
        {
            case sf::Event::Resized: 
                //window.setView(calcView(WINDOW_SIZE, 1.f, 1.f));
                break;
            case sf::Event::MouseWheelScrolled:
            {
                if (event.mouseWheelScroll.delta > 0 && getRadius() < AppConfig::BrushSizeMax){
                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                        changeSize(AppConfig::BrushSizeDelta);
                }
                    
                else if (event.mouseWheelScroll.delta < 0 && getRadius() > AppConfig::BrushSizeMin)
                {
                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                        changeSize(-AppConfig::BrushSizeDelta);
                }
                    
                /* Check if brush size is within threshold after mouse wheel event */
                if (getRadius() < AppConfig::BrushSizeMin)
                    setRadius(AppConfig::BrushSizeMin);

                if (getRadius() > AppConfig::BrushSizeMax)
                    setRadius(AppConfig::BrushSizeMax);
                
                /* Update the brush's origin after resize */
                setOriginToCenter();
                std::cout << "brush.size: " << getRadius() << "\n";
            }
            case sf::Event::MouseMoved:
                setPosition(AppConfig::mousePosition);
                break;
            
            case sf::Event::MouseButtonPressed:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    ;
                break;
            case sf::Event::MouseButtonReleased:
                break;
        }
    }

    void Brush::update(float deltaTime)
    {
        
    }

    void Brush::render(sf::RenderWindow& window)
    {
        window.draw(getShape());
    }

	void Brush::setOriginToCenter()
	{
		m_shape.setOrigin(
			(m_shape.getLocalBounds().left + m_shape.getLocalBounds().width) / 2.0f,
			(m_shape.getLocalBounds().top + m_shape.getLocalBounds().height) / 2.0f);
	}

	void Brush::build(float size)
	{
		m_shape.setRadius(size);
		m_shape.setFillColor(sf::Color(0, 0, 255, 100));
	}
}