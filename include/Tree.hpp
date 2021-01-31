
#ifndef SP_HASH_TREE_HPP
#define SP_HASH_TREE_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <array>
#include <iterator>
#include <vector>
#include <math.h>

namespace sp_hash
{
    template <size_t N>
    class Node
    {
    public:
        typedef std::array< Node<N>*, N >   NodeArray;
        typedef std::vector< Node<N>* >     NodeVector;

        Node(const sf::FloatRect& bounds);
        ~Node();

        NodeVector search(const sf::FloatRect& bounds);
        const sf::FloatRect& getBounds() const;
        void partition(sf::Uint64 levels, const sf::FloatRect& parentBounds);
        void setBounds(const sf::FloatRect& bounds);
    
    private:
        NodeArray m_childNodes;
        sf::FloatRect m_bounds;
    };


    template <size_t N>
    class Tree
    {
    public:
        typedef std::array< Node<N>*, N >   NodeArray;
        typedef std::vector< Node<N> >      NodeVector;

        Tree(const sf::FloatRect& rect, sf::Uint64 levels = 0);
        ~Tree();

        NodeVector search(const sf::FloatRect& searchBounds);
        void partition(sf::Uint64 levels);
        void setBounds(const sf::FloatRect& bounds);
    
    private:
        Tree<N> *m_root;
    };


    template <size_t N>
    Node<N>::Node(const sf::FloatRect& bounds)
    : m_bounds(bounds)
    { m_childNodes.fill(nullptr); }

    template <size_t N>
    Node<N>::~Node()
    { foreach(m_childNodes, [](Node<N> *node){ if(node) delete node; }); }

    template <size_t N>
    typename Node<N>::NodeVector Node<N>::search(const sf::FloatRect& bounds)
    {
        typename Node<N>::NodeVector f;
        for(const auto& node : m_childNodes)
        {
            if(!node)
                continue;

            if(node->getBounds().intersects(bounds))
            {
                NodeVector nodes = node->search(bounds);
                if(nodes.empty())
                    f.emplace_back(node);
                f.insert(f.end(),
                    std::make_move_iterator(nodes.begin()),
                    std::make_move_iterator(nodes.end())
                );
                nodes.erase(nodes.begin(), nodes.end());
            }
        }

        return f;
    }

    template <size_t N>
    void Node<N>::partition(sf::Uint64 levels, const sf::FloatRect& bounds)
    {
        if(levels <= 0)
            return;

        float x = m_bounds.left;
        float y = m_bounds.top;
        float w = m_bounds.width*std::round(1/N);
        float h = m_bounds.height*std::round(1/N);

        for(size_t i = 0; i < m_childNodes.size(); ++i)
        {
            sf::FloatRect q(x*i, y*i, w, h);
            m_childNodes[i] = new Node<N>();
        }

        sf::FloatRect q0(x, y, w, h);
        sf::FloatRect q1(x+w, y, w, h);
        sf::FloatRect q2(x, y+h, w, h);
        sf::FloatRect q3(x+w, y+h, w, h);

        m_childNodes[0];
    }

    // typedef Tree<2> BinaryTree;
    // typedef Tree<4> Quadtree;
    // typedef Tree<8> Octree;
}

#endif 