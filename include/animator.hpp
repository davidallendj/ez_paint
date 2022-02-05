#pragma once

#include "tree.hpp"

namespace fu 
{
    template <size_t N>
    class Animator
    {
    public:
        Animator(StaticTree<N>& tree);

    private:
        StaticTree<N>& m_tree;
        Node<N> *m_currentNode;
        sf::Color m_color;
    };
}
