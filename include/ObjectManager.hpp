#ifndef SP_HASH_OBJECT_MANAGER_HPP
#define SP_HASH_OBJECT_MANAGER_HPP

#include "App.hpp"
#include "Object.hpp"
#include "Utils.hpp"
#include "ThreadPool.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>

namespace sp_hash
{
    class ObjectManager : IObject
    {
    public:
        typedef IObject* PtrType;
        typedef std::vector<PtrType> ObjectVector;
        ObjectManager();
        ObjectManager(sf::RenderWindow& window, sf::Event& event);
        virtual ~ObjectManager(){}

        // template <typename T, typename...Args>
        // void createObject(T&& object, Args...args);
        PtrType createObject(PtrType object);
        void removeObject(std::size_t index);
        void loop();

        void handleEvents(sf::RenderWindow& window, sf::Event& event);
        void update(float deltaTime);
        void render(sf::RenderWindow& window);

    private:
        bool m_isRunning;
        ObjectVector m_objects;
        ThreadPool m_threadPool;
        sf::RenderWindow& m_window;
        sf::Event& m_event;
        sf::Clock m_clock;
        sf::Time m_time;

        // Dragging vars
        sf::Vector2f m_dragPosition;
        float m_cameraSensitivity;
    };
    
    ObjectManager::ObjectManager(
        sf::RenderWindow& window,
        sf::Event& event
    )
    : m_isRunning(true)
    , m_threadPool(4)
    , m_window(window)
    , m_event(event)
    , m_clock(sf::Clock())
    , m_time(sf::Time())
    , m_dragPosition(sf::Vector2f())
    , m_cameraSensitivity(0.30)
    {}

    // template <typename T, typename...Args>
    // void ObjectManager::createObject(T&& object, Args...args)
    // { m_objects.emplace_back(std::make_unique<T>(args...)); }

    ObjectManager::PtrType ObjectManager::createObject(ObjectManager::PtrType object)
    { 
        m_objects.emplace_back(object); 
        return object;
    }

    void ObjectManager::removeObject(std::size_t index)
    { 
        //m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), index)); 
    }

    void ObjectManager::loop()
    {
        // TODO: Run loop over a time step, ideally 60 fps or 16.6 ms per frame
        while(m_isRunning)
        {
            if(m_window.hasFocus())
            {
                sf::Time deltaTime = m_clock.restart();
                // m_threadPool.enqueue(
                //     [this](sf::Event& event){ 
                //         this->handleEvents(this->m_window, event);
                //     }, m_event
                // );
                // auto update = m_threadPool.enqueue(
                //     [this](float delta){
                //         this->update(delta);
                //     }, m_time.asSeconds()
                // );
                // m_threadPool.enqueue(
                //     [this]{
                //         this->render(this->m_window);
                //     }
                // );
                handleEvents(m_window, m_event);
                update(deltaTime.asSeconds());
                render(m_window);
            }
        }
    }

    void ObjectManager::handleEvents(sf::RenderWindow& window, sf::Event& event)
    {
        // Only call handleEvents() if events are queued
        while(window.pollEvent(event))
        {
            /* Close Button Event */
			if (event.type == sf::Event::Closed)
				m_isRunning = false;

            if (event.type == sf::Event::MouseMoved)
            {
                App::mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                {
                    // Get delta 
                    const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    // const sf::Vector2f deltaPos = App::lastMousePosition - newPos;
                    sf::Vector2f deltaPos = (m_dragPosition - App::mousePosition)*m_cameraSensitivity;

                    // Move our view accordingly and update the window
                    sf::View view = window.getView();
                    view.setCenter(view.getCenter() + deltaPos);
                    window.setView(view);

                    // Save the new position as the old one
                    // We're recalculating this, since we've changed the view
                    App::lastMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                }
                
            }

            if(event.type == sf::Event::MouseButtonPressed)
            {
                m_dragPosition = App::mousePosition;

            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {

                // sf::Vector2f deltaPos = m_dragPosition - App::mousePosition;
                // sf::View view = window.getDefaultView();
                // view.setCenter(view.getCenter() + deltaPos);
                // window.setView(view);
            }
                
            foreach(m_objects, [&window, &event](PtrType& object){
            object->handleEvents(window, event);
            });
        }
    }

    void ObjectManager::update(float deltaTime)
    {
        foreach(m_objects, [&deltaTime](PtrType& object){
            object->update(deltaTime);
        });
    }

    void ObjectManager::render(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 200, 200));
        foreach(m_objects, [&window](PtrType& object){
            object->render(window);
        });
        window.display();
    }

}

#endif // SP_HASH_OBJECT_MANAGER_HPP