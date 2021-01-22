
#include "Brush.hpp"
#include "App.hpp"
#include "Utils.hpp"


#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sp_hash
{

    Brush::Brush(float radius, const sf::Vector2f& position, bool setOriginToCenter)
    : Object::Object(radius, position, setOriginToCenter)
    { }

    void Brush::handleEvents(sf::RenderWindow& window, sf::Event& event)
    {
        bool isCtrlPressed = false;

        if (event.type == sf::Event::Resized){}
            //window.setView(calcView(WINDOW_SIZE, 1.f, 1.f));

        /* Mouse Wheel Events */
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            if (event.mouseWheelScroll.delta > 0 && getRadius() < App::BrushSizeMax){
                changeSize(App::BrushSizeDelta);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    sf::Vector2i wheelDelta = sf::Vector2i(
                        event.mouseWheelScroll.x,
                        event.mouseWheelScroll.y
                    );
                    zoomViewAt(wheelDelta, window, 1.0);
                }
            }
                
            else if (event.mouseWheelScroll.delta < 0 && getRadius() > App::BrushSizeMin)
                changeSize(-App::BrushSizeDelta);
                
            /* Check if brush size is within threshold after mouse wheel event */
            if (getRadius() < App::BrushSizeMin)
                setRadius(App::BrushSizeMin);

            if (getRadius() > App::BrushSizeMax)
                setRadius(App::BrushSizeMax);
            
            /* Update the brush's origin after resize */
            setOriginToCenter();
            std::cout << "brush.size: " << getRadius() << "\n";
        }

        /* Move moved event */
        if(event.type == sf::Event::MouseMoved)
        {
            setPosition(App::mousePosition);
        }

        /* Mouse button events */
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                
            }
        }

        if(event.type == sf::Event::MouseButtonReleased)
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {

            }
        }
        
    }

    void Brush::update(float deltaTime)
    {
        
    }

    void Brush::render(sf::RenderWindow& window)
    {
        window.draw(getShape());
    }
}