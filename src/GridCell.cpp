
#include "GridCell.hpp"
#include <cmath>

namespace sp_hash
{
	GridCell::GridCell()
	{ buildGridCell(); }

	GridCell::GridCell(const sf::Vector2f& position)
	{
		m_shape.setPosition(position);
		buildGridCell();
	}
	
	sf::Vector2u GridCell::getPosition() const
	{
		return sf::Vector2u(
			std::floor(m_shape.getPosition().x / m_shape.getSize().x),
			std::floor(m_shape.getPosition().y / m_shape.getSize().y));
	}

	void GridCell::buildGridCell()
	{
		m_shape.setOutlineColor(sf::Color(255, 0, 0, 150));
		m_shape.setOutlineThickness(1);
		m_shape.setSize(sf::Vector2f(32, 32));
	}
}
