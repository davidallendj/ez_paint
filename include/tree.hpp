
#ifndef FU_TREE_HPP
#define FU_TREE_HPP

#include "config.hpp"
#include "collision.hpp"
#include "utils.hpp"

#include <array>
#include <iterator>
#include <type_traits>
#include <vector>
#include <math.h>

#include <SFML/Config.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace fu
{
	template <typename T>
	concept IsRect = std::is_same<T, sf::FloatRect>::value || std::is_base_of_v<sf::Shape, T>;

	template <size_t N>
	class Node
	{ 
	public:
		typedef std::array< Node<N>*, N >   NodeArray;
		typedef std::vector< Node<N>* >     NodeVector;

		explicit Node(const sf::FloatRect& bounds);
		virtual ~Node();

		const sf::FloatRect& getBounds() const;
		const NodeArray& getNodes() const;
		void setBounds(const sf::FloatRect& bounds);

		NodeVector search(const sf::CircleShape& targetShape);
		NodeVector search(const sf::FloatRect& targetBounds);
	
		virtual void partition(sf::Uint64 height, const sf::FloatRect& parentBounds){};

	protected:
		NodeArray m_childNodes;
		sf::FloatRect m_bounds;
	};

	template <size_t N>
	Node<N>::Node(const sf::FloatRect& bounds)
	: m_bounds(bounds)
	{ m_childNodes.fill(nullptr); }

	template <size_t N>
	Node<N>::~Node()
	{ 
		// foreach(m_childNodes, [](Node<N> *node){ 
		//     if(node){
		//         delete node;
		//         node = nullptr; 
		//     } 
		// });
	}

	template <size_t N>
	const sf::FloatRect& Node<N>::getBounds() const
	{ return m_bounds; }

	template <size_t N>
	const typename Node<N>::NodeArray& Node<N>::getNodes() const
	{ return m_childNodes; }


	template <size_t N>
	void Node<N>::setBounds(const sf::FloatRect &bounds)
	{ m_bounds = bounds; }

	template <size_t N>
	typename Node<N>::NodeVector Node<N>::search(const sf::CircleShape& targetShape)
	{
		NodeVector f;
		for(const auto& node : m_childNodes)
		{
			if(!node)
				continue;

			// if(node->getBounds().intersects(targetBounds))
			if(Collision::CircleTest(targetShape, node->getBounds()))
			{
				NodeVector nodes = node->search(targetShape);
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
	typename Node<N>::NodeVector Node<N>::search(const sf::FloatRect& targetBounds)
	{
		NodeVector f;
		for(const auto& node : m_childNodes)
		{
			if(!node)
				continue;

			// if(node->getBounds().intersects(targetBounds))
			if(node->getBounds().intersects(targetBounds))
			{
				NodeVector nodes = node->search(targetBounds);
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

	template <sf::Uint64 N>
	class StaticTree
	{
	public:
		typedef std::array< Node<N>*, N >   NodeArray;
		typedef std::vector< Node<N>* >     NodeVector;

		// template <typename Node>
		StaticTree(const sf::FloatRect& bounds, sf::Uint64 depth = 1, Node<N> *node = nullptr);
		virtual ~StaticTree();

		void partition(sf::Uint64 depth);
		void setBounds(const sf::FloatRect& bounds);

		NodeVector search(const sf::CircleShape& targetShape);
		NodeVector search(const sf::FloatRect& targetBounds);
	
	protected:
		Node<N> *m_root;
		sf::Uint64 m_recommendedPartitions;
	};

	template <sf::Uint64 N>
	// template <typename Node>
	StaticTree<N>::StaticTree(const sf::FloatRect& bounds, sf::Uint64 depth, Node<N> *root)
	// : m_root(root)
	{
		m_root = root;
		partition( (depth > 0) ? depth : 0 );
	}

	template <sf::Uint64 N>
	StaticTree<N>::~StaticTree<N>()
	{
		// if(m_root)
		// {
		//     delete m_root;// do some shit
		//     m_root = nullptr;
		// }
	}

	template <sf::Uint64 N>
	typename StaticTree<N>::NodeVector 
	StaticTree<N>::search(const sf::FloatRect& targetBounds)
	{        
		NodeVector f;
		for(const auto& node : m_root->getNodes())
		{
			if(!node)
				continue;

			if(node->getBounds().intersects(targetBounds))
			{
				NodeVector nodes = node->search(targetBounds);
				f.insert(f.end(),
					std::make_move_iterator(nodes.begin()),
					std::make_move_iterator(nodes.end())
				);
				nodes.erase(nodes.begin(), nodes.end());
			}
		}

		return f;
	}

	template <sf::Uint64 N>
	typename StaticTree<N>::NodeVector
	StaticTree<N>::search(const sf::CircleShape& targetShape)
	{
		NodeVector f;
		for(const auto& node : m_root->getNodes())
		{
			if(!node)
				continue;

			if(Collision::CircleTest(targetShape, node->getBounds()))
			{
				NodeVector nodes = node->search(targetShape);
				f.insert(f.end(),
					std::make_move_iterator(nodes.begin()),
					std::make_move_iterator(nodes.end())
				);
			}
		}

		return f;
	}

	template <sf::Uint64 N>
	void StaticTree<N>::partition(sf::Uint64 height)
	{
		sf::FloatRect bounds = m_root->getBounds();
		m_root->partition(height, bounds);
	}

	template <sf::Uint64 N>
	void StaticTree<N>::setBounds(const sf::FloatRect &bounds)
	{ m_root->setBounds(bounds); }

	// TODO: Need general implementation of partition() to achieve this
	// typedef StaticTree<2> BinaryTree;
	// typedef StaticTree<4> Quadtree;
	// typedef StaticTree<8> Octree;

	class QuadtreeNode : public Node<4>
	{
	public:
		explicit QuadtreeNode(const sf::FloatRect& bounds);
		virtual ~QuadtreeNode();

		void partition(sf::Uint64 depth, const sf::FloatRect& parentBounds);
	};

	class Quadtree : public StaticTree<4>
	{
	public:
		Quadtree(const sf::FloatRect& bounds, sf::Uint64 depth = config::TreePartitions);
		virtual ~Quadtree();
	};

	class OctreeNode : public Node<8>
	{
	public:
		explicit OctreeNode(const sf::FloatRect& bounds);
		virtual ~OctreeNode();

		void partition(sf::Uint64 depth, const sf::FloatRect& parent);
	};

	class Octree : public StaticTree<8>
	{
	public:
		Octree(const sf::FloatRect& bounds, sf::Uint64 depth = config::TreePartitions);
		virtual ~Octree();
	};
}

#endif 