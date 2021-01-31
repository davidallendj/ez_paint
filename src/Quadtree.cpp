
#include "Quadtree.hpp"
#include "Grid.hpp"
#include "Utils.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>
#include <iterator>
#include <memory>

namespace sp_hash
{
    QuadtreeNode::QuadtreeNode(const sf::FloatRect& rect)
    : m_childNodes{ nullptr, nullptr, nullptr, nullptr }
    , m_bounds(rect)
    {}

    QuadtreeNode::~QuadtreeNode()
    {
        std::for_each(m_childNodes.begin(), m_childNodes.end(), [](QuadtreeNode *node){ if(node) delete node; });
    }

    QuadtreeNode::QuadtreeNodeVector QuadtreeNode::search(const sf::FloatRect& bounds)
    {
        // QuadtreeNode *nodePtr = this;
        QuadtreeNodeVector found;
        for(const auto& node : m_childNodes)
        {
            if(!node)
                continue;

            if(node->getBounds().intersects(bounds))
            {

                // Push all child nodes with intersection as well
                QuadtreeNodeVector nodes = node->search(bounds);

                // Push if no child nodes (indicates bottom node)
                if(nodes.empty())
                    found.emplace_back(node);

                found.insert(found.end(),
                    std::make_move_iterator(nodes.begin()),
                    std::make_move_iterator(nodes.end())
                );

                // Erase all pointers to old vector
                nodes.erase(nodes.begin(), nodes.end());
            }
        }

        // return nodePtr;
        return found;
    }

    void QuadtreeNode::partition(sf::Uint64 levels, const sf::FloatRect& rect)
    {
        // Exit loop if no more levels
        if(levels <= 0)
            return;
        
        float x = m_bounds.left;
        float y = m_bounds.top;
        float w = m_bounds.width*0.5;
        float h = m_bounds.height*0.5;

        sf::FloatRect q0(x, y, w, h);
        sf::FloatRect q1(x+w, y, w, h);
        sf::FloatRect q2(x, y+h, w, h);
        sf::FloatRect q3(x+w, y+h, w, h);

        m_childNodes[0] = new QuadtreeNode(q0);
        m_childNodes[1] = new QuadtreeNode(q1);
        m_childNodes[2] = new QuadtreeNode(q2);
        m_childNodes[3] = new QuadtreeNode(q3);

        m_childNodes[0]->partition(levels-1, q0);
        m_childNodes[1]->partition(levels-1, q1);
        m_childNodes[2]->partition(levels-1, q2);
        m_childNodes[3]->partition(levels-1, q3);
    }

    Quadtree::Quadtree(const sf::FloatRect& bounds, sf::Uint64 levels)
    : m_root(new QuadtreeNode(bounds))
    {
        if(levels > 0) 
            partition(levels); 
    }

    Quadtree::QuadtreeNodeVector Quadtree::search(const sf::FloatRect& brushBounds)
    {
        // std::cout << "brush.bounds: " << brushBounds << "\n";

        // Iterate through all child nodes one level and continue iterating
        // on found child nodes.
        // QuadtreeNode *nodePtr = m_root;
        QuadtreeNodeVector found;
        for(const auto& node : m_root->getNodes())
        {
            if(!node)
                continue;

            if(node->getBounds().intersects(brushBounds))
            {
                // QuadtreeNode *q = node->search(brushBounds);
                // childNodes.emplace_back((q) ? q : node);
                // nodePtr = (q) ? q : node;
                found = node->search(brushBounds);
                // QuadtreeNodeVector nodes = node->search(brushBounds);
                // found.insert(found.end(),
                //     std::make_move_iterator(nodes.begin() + nodes.size()),
                //     std::make_move_iterator(nodes.end()) 
                // );
            }
        }
        
        return found;
    }

    void Quadtree::partition(sf::Uint64 levels)
    {
        // Take grid bounds and add vertices to root vertex array
        // then, recursively call levels until (levels <= 0).
        sf::FloatRect bounds = m_root->getBounds();
        m_root->partition(levels, bounds);
    }
}