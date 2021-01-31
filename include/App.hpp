#ifndef SP_HASH_APP_HPP
#define SP_HASH_APP_HPP

#include "AppConfig.hpp"
#include "Message.hpp"
#include "Object.hpp"
#include "Utils.hpp"
#include "ThreadPool.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui-SFML.h"

namespace sp_hash
{
    class App : public MessageReceiver<>, Object
    {
    public:
        typedef Object* PtrType;
        typedef std::vector<PtrType> ObjectVector;
        App(sf::RenderWindow& window);
        virtual ~App();

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
        sf::Event m_event;
        sf::Clock m_clock;
        sf::Time m_time;

        // Dragging vars
        sf::Vector2f m_dragPosition;
        float m_zoom;
        float m_cameraSensitivity;
        float m_zoomSensitivity;
    };
    
    App::App(sf::RenderWindow& window)
    : m_isRunning(true)
    , m_threadPool(4)
    , m_window(window)
    , m_event()
    , m_clock(sf::Clock())
    , m_time(sf::Time())
    , m_dragPosition(sf::Vector2f())
    , m_cameraSensitivity(0.30)
    , m_zoomSensitivity(0.10)
    {
        m_window.setFramerateLimit(60);
        m_window.setVerticalSyncEnabled(false);
        ImGui::SFML::Init(m_window);
    }

    App::~App()
    {
        ImGui::SFML::Shutdown();
    }

    // template <typename T, typename...Args>
    // void App::createObject(T&& object, Args...args)
    // { m_objects.emplace_back(std::make_unique<T>(args...)); }

    App::PtrType App::createObject(App::PtrType object)
    { 
        m_objects.emplace_back(object); 
        return object;
    }

    void App::removeObject(std::size_t index)
    { 
        //m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), index)); 
    }

    void App::loop()
    {
        // TODO: Run loop over a time step, ideally 60 fps or 16.6 ms per frame
        sf::Time lastUpdate = sf::Time::Zero;
        while(m_isRunning)
        {
            if(m_window.hasFocus())
            {
                sf::Time dt = m_clock.restart();
                // lastUpdate += dt;
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
                // while(lastUpdate > AppConfig::TimePerFrame)
                // {
                //     lastUpdate -= AppConfig::TimePerFrame;
                    
                // }
                handleEvents(m_window, m_event);
                update(dt.asSeconds());
                render(m_window);
            }
        }
    }

    void App::handleEvents(sf::RenderWindow& window, sf::Event& event)
    {
        // Only call handleEvents() if events are queued
        while(window.pollEvent(event))
        {
            // processMessages();

            /* Handle ImGui events */
            ImGui::SFML::ProcessEvent(event);
            switch(event.type)
            {
                case sf::Event::Closed:
                    m_isRunning = false;
                    break;
                case sf::Event::MouseMoved:
                {
                    AppConfig::mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                    {
                        // Get delta 
                        const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                        // const sf::Vector2f deltaPos = AppConfig::lastMousePosition - newPos;
                        sf::Vector2f deltaPos = (m_dragPosition - AppConfig::mousePosition)*m_cameraSensitivity;

                        // Move our view accordingly and update the window
                        sf::View view = window.getView();
                        view.setCenter(view.getCenter() + deltaPos);
                        window.setView(view);

                        // Save the new position as the old one
                        // We're recalculating this, since we've changed the view
                        AppConfig::lastMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    }
                    break;
                }
                case sf::Event::MouseButtonPressed:
                    m_dragPosition = AppConfig::mousePosition;
                    break;
                case sf::Event::MouseButtonReleased:
                    break;
                case sf::Event::Resized:
                    // sf::VideoMode vm = sf::VideoMode::getDesktopMode();
                    // sf::Vector2u v = sf::Vector2u(vm.width, vm.height);
                    // sf::Vector2u w = AppConfig::WindowSize;
                    // sf::Vector2f scale = sf::Vector2f(v.x/w.x, v.y/w.y);
                    break;
                
                case sf::Event::LostFocus:
                    // Stop processing while window not focused
                    break;
                case sf::Event::GainedFocus:
                    // Continue processing when focused
                    break;
                case sf::Event::MouseWheelScrolled:
                {
                    if (event.mouseWheelScroll.delta > 0)
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_zoom > AppConfig::CameraZoomMin)
                        {
                            sf::Vector2i p = window.mapCoordsToPixel(AppConfig::mousePosition);
                            zoomViewAt(p, window, 1.00 - m_zoomSensitivity);
                            m_zoom -= m_zoomSensitivity;
                        }
                    }
                    else if (event.mouseWheelScroll.delta < 0)
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_zoom < AppConfig::CameraZoomMax)
                        {
                            sf::Vector2i p = window.mapCoordsToPixel(AppConfig::mousePosition);
                            zoomViewAt(p, window, 1.00 + m_zoomSensitivity);
                            m_zoom += m_zoomSensitivity;
                        }
                    }
                    break;
                }
            }
                
            foreach(m_objects, [&window, &event](PtrType& object){
                object->handleEvents(window, event);
            });
        }
    }

    void App::update(float deltaTime)
    {
        /* Update ImGui::SFML */
        ImGui::SFML::Update(m_window, m_clock.restart());
        foreach(m_objects, [&deltaTime](PtrType& object){
            object->update(deltaTime);
        });
    }

    void App::render(sf::RenderWindow& window)
    {
        window.clear(sf::Color(200, 200, 200));
        foreach(m_objects, [&window](PtrType& object){
            object->render(window);
        });
        ImGui::SFML::Render(window);
        window.display();
    }

}

#endif // SP_HASH_APP_HPP