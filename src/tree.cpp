
#include "tree.hpp"
#include "config.hpp"
#include "log.hpp"
#include "format.hpp"

#include <bits/ranges_algo.h>
#include <SFML/Graphics/Rect.hpp>
#include <fmt/core.h>

namespace ez_paint
{
	QuadtreeNode::QuadtreeNode(const sf::FloatRect& bounds)
	: Node<4>::Node(bounds)
	{ }

	QuadtreeNode::~QuadtreeNode()
	{ 
		Node<4>::~Node(); 
	}

	Quadtree::Quadtree(const sf::FloatRect& bounds, sf::Uint64 depth)
	: StaticTree<4>::StaticTree(bounds, depth, new QuadtreeNode(bounds))
	{ 
		log::info("Created 'quadtree'");
	}

	Quadtree::~Quadtree()
	{ 
		StaticTree<4>::~StaticTree();
		log::info("Destroyed 'quadtree'"); 
	}

	void QuadtreeNode::partition(sf::Uint64 depth, const sf::FloatRect& parentBounds)
	{
		// Exit loop if no more depth
		if(depth <= 0)
			return;
		
		float x = m_bounds.left;
		float y = m_bounds.top;
		float w = m_bounds.width*0.5f;
		float h = m_bounds.height*0.5f;

		sf::FloatRect q0(x, y, w, h);
		sf::FloatRect q1(x+w, y, w, h);
		sf::FloatRect q2(x, y+h, w, h);
		sf::FloatRect q3(x+w, y+h, w, h);

		m_childNodes[0] = new QuadtreeNode(q0);
		m_childNodes[1] = new QuadtreeNode(q1);
		m_childNodes[2] = new QuadtreeNode(q2);
		m_childNodes[3] = new QuadtreeNode(q3);

		m_childNodes[0]->partition(depth-1, q0);
		m_childNodes[1]->partition(depth-1, q1);
		m_childNodes[2]->partition(depth-1, q2);
		m_childNodes[3]->partition(depth-1, q3);
		
		// log::info("Create quadtree partition (depth {}, {})", depth, ez_paint::format::to_string(parentBounds));
	}

	OctreeNode::OctreeNode(const sf::FloatRect& bounds)
	: Node<8>::Node(bounds)
	{ log::info("Created 'octree'"); }

	OctreeNode::~OctreeNode()
	{ Node<8>::~Node(); }

	Octree::Octree(const sf::FloatRect& bounds, sf::Uint64 depth)
	: StaticTree<8>::StaticTree(bounds, depth, new OctreeNode(bounds))
	{ }

	Octree::~Octree()
	{ StaticTree<8>::~StaticTree(); }

	void OctreeNode::partition(sf::Uint64 depth, const sf::FloatRect& parent)
	{
		// Exit loop if no more depth
		if(depth <= 0)
			return;
		
		float x = m_bounds.left;
		float y = m_bounds.top;
		float w = m_bounds.width*0.25;
		float h = m_bounds.height*0.50;

		sf::FloatRect q0(x, y, w, h);
		sf::FloatRect q1(x+w, y, w, h);
		sf::FloatRect q2(x+2*w, y, w, h);
		sf::FloatRect q3(x+3*w, y, w, h);
		sf::FloatRect q4(x, y+h, w, h);
		sf::FloatRect q5(x+w, y+h, w, h);
		sf::FloatRect q6(x+2*w, y+h, w, h);
		sf::FloatRect q7(x+3*w, y+h, w, h);

		m_childNodes[0] = new OctreeNode(q0);
		m_childNodes[1] = new OctreeNode(q1);
		m_childNodes[2] = new OctreeNode(q2);
		m_childNodes[3] = new OctreeNode(q3);
		m_childNodes[4] = new OctreeNode(q4);
		m_childNodes[5] = new OctreeNode(q5);
		m_childNodes[6] = new OctreeNode(q6);
		m_childNodes[7] = new OctreeNode(q7);

		m_childNodes[0]->partition(depth-1, q0);
		m_childNodes[1]->partition(depth-1, q1);
		m_childNodes[2]->partition(depth-1, q2);
		m_childNodes[3]->partition(depth-1, q3);
		m_childNodes[4]->partition(depth-1, q4);
		m_childNodes[5]->partition(depth-1, q5);
		m_childNodes[6]->partition(depth-1, q6);
		m_childNodes[7]->partition(depth-1, q7);
	}

} // namespace ez_paint