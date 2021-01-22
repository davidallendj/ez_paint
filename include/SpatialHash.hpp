
#ifndef SP_HASH_HASH_TABLE_HPP
#define SP_HASH_HASH_TABLE_HPP

#include "SpriteBatch.hpp"
#include <unordered_map>
#include <iostream>
#include <thread>
#include <mutex>

namespace sp_hash
{
	class Grid;
	class GridCell;
	class Object;

	class SpatialHash
	{
	public:
		SpatialHash(Grid& grid, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));

		// Not performing actually hashing since nothing is being adding to the hash_table
		void hash(Object& object, SpriteBatch& batch);
		void clear();
		const std::unordered_map<const sf::FloatRect*, Object*>& getHashTable() const;

	private:
		std::unordered_map<const sf::FloatRect*, Object*> m_hashTable;
		Grid& m_grid;
		sf::Vector2u m_gridCellSize;
		sf::Vector2u m_gridSize;
		std::mutex m_mutex;
	};
}


#endif // SP_HASH_HASH_TABLE_HPP