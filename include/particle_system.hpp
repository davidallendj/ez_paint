#ifndef FU_PARTICLE_SYSTEM_HPP
#define FU_PARTICLE_SYSTEM_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

namespace fu
{
    class ParticleSystem : public sf::Drawable, public sf::Transformable
    {
    public:
        ParticleSystem(sf::Uint32 count)
        : m_particles(count)
        , m_vertexArray(sf::Points, count)
        , m_lifetime(sf::seconds(3.f))
        , m_emitter(0.f, 0.f)
        {}
    
    void setEmitter(sf::Vector2f position)
    { m_emitter = position; }

    void update(sf::Time elapsed)
    {
        for(sf::Uint64 i = 0; i < m_particles.size(); ++i)
        {
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            if(p.lifetime <= sf::Time::Zero)
                resetParticle(i);

            m_vertexArray[i].position += p.velocity * elapsed.asSeconds();

            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertexArray[i].color.a = static_cast<sf::Uint8>(ratio);
        }
        

    }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            states.texture = nullptr;
            target.draw(m_vertexArray, states);
        };
    
        struct Particle
        {
            sf::Vector2f velocity;
            sf::Time lifetime;
        };

        void resetParticle(size_t index)
        {
            float angle = (std::rand() % 360) * 3.14f / 180.f;
            float speed = (std::rand() % 50) + 50.f;
            m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
            m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

            m_vertexArray[index].position = m_emitter;
        }

        std::vector<Particle> m_particles;
        sf::VertexArray m_vertexArray;
        sf::Time m_lifetime;
        sf::Vector2f m_emitter;
    };
}

// int main()
// {
//     // create the window
//     sf::RenderWindow window(sf::VideoMode(512, 256), "Particles");

//     // create the particle system
//     ParticleSystem particles(1000);

//     // create a clock to track the elapsed time
//     sf::Clock clock;

//     // run the main loop
//     while (window.isOpen())
//     {
//         // handle events
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if(event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // make the particle system emitter follow the mouse
//         sf::Vector2i mouse = sf::Mouse::getPosition(window);
//         particles.setEmitter(window.mapPixelToCoords(mouse));

//         // update it
//         sf::Time elapsed = clock.restart();
//         particles.update(elapsed);

//         // draw it
//         window.clear();
//         window.draw(particles);
//         window.display();
//     }

//     return 0;
// }

#endif