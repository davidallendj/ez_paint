
#pragma once

#include "Utils.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

namespace fu
{
    class SceneNode
    {
    public:
        // functions to transform node

        void draw(sf::RenderTarget& target, const sf::Transform& parent) const
        {
            sf::Transform combined = parent * m_transform;
            onDraw(target, combined);
            foreach(m_children, [](SceneNode *node){

            });
        }
    
    private:
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const = 0;
        sf::Transform m_transform;
        std::vector<SceneNode*> m_children;
    };

    class SpriteNode
    {
    public:

    private:
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
        { target.draw(m_sprite, transform); }
        sf::Sprite m_sprite;
    }
}
