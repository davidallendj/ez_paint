
#ifndef SP_HASH_QUADTREE_HPP
#define SP_HASH_QUADTREE_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <array>
#include <memory>

namespace sp_hash
{
    class Grid;
    class QuadtreeNode
    {
    public:
        typedef std::array<QuadtreeNode*, 4> QuadtreeNodeArray;
        typedef std::vector<QuadtreeNode*> QuadtreeNodeVector;

        QuadtreeNode(const sf::FloatRect& rect);
        ~QuadtreeNode();

        QuadtreeNodeVector search(const sf::FloatRect& bounds);
        const sf::FloatRect& getBounds() const;
        const QuadtreeNodeArray& getNodes() const;
        void partition(sf::Uint64 levels, const sf::FloatRect& parentBounds);
        void setBounds(const sf::FloatRect& bounds);
    
    private:
        QuadtreeNodeArray m_childNodes;
        sf::FloatRect m_bounds;
    };

    inline const sf::FloatRect& QuadtreeNode::getBounds() const
    { return m_bounds; }

    inline const QuadtreeNode::QuadtreeNodeArray& QuadtreeNode::getNodes() const
    { return m_childNodes; }

    inline void QuadtreeNode::setBounds(const sf::FloatRect &bounds)
    { m_bounds = bounds; }

    class Quadtree
    {
    public:
        typedef std::vector<QuadtreeNode*> QuadtreeNodeVector;

        Quadtree(const sf::FloatRect& bounds, sf::Uint64 levels = 0);
        QuadtreeNodeVector search(const sf::FloatRect& brushBounds);
        void partition(sf::Uint64 levels);
        void setBounds(const sf::FloatRect& bounds);
    
    private:
        QuadtreeNode *m_root;
    };

    inline void Quadtree::setBounds(const sf::FloatRect &bounds)
    { m_root->setBounds(bounds); }
}

#endif