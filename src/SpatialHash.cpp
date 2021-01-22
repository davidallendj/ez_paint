
#include "SpatialHash.hpp"
#include "Grid.hpp"
#include "GridCell.hpp"
#include "Object.hpp"
#include "SpriteBatch.hpp"
#include "Collision.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <atomic>

namespace sp_hash
{
	SpatialHash::SpatialHash(Grid& grid, const sf::Vector2u& cellSize)
	: m_grid(grid)
	, m_gridSize(sf::Vector2u(grid.getSize().x, grid.getSize().y))
	, m_gridCellSize(cellSize)
	, m_mutex()
	{}

	void SpatialHash::clear()
	{ m_hashTable.clear(); }

	inline const std::unordered_map<const sf::FloatRect*, Object*>& SpatialHash::getHashTable() const
	{ return m_hashTable; }

	void SpatialHash::hash(Object& object, SpriteBatch& batch)
	{
		sf::FloatRect bounds;
		for (unsigned int j = 0; j < m_gridSize.x; ++j)
		{
			for (unsigned int i = 0; i < m_gridSize.y; ++i)
			{
				bounds = batch.getCellBounds(sf::Vector2u(i, j));
				if (object.getShape().getGlobalBounds().intersects(bounds)) // Collision::CircleTest(object.getShape(), batch.getCellShape(sf::Vector2u(i, j)))
				{
					//m_hashTable[&bounds] = _object;
					if (object.getTrigger()) 
						batch.setCellColor(sf::Vector2u(j, i), m_grid.getPaletteRef().getCurrentSelection().getColor());
					else if (!object.getTrigger()) batch.setCellColor(sf::Vector2u(j, i), sf::Color(255, 255, 255, 255));
				}
			}
		}	
	}
}



