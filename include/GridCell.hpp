
#ifndef SP_HASH_GRID_CELL_HPP
#define SP_HASH_GRID_CELL_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sp_hash
{
	class GridCell
	{
	public:
		GridCell();
		explicit GridCell(const sf::Vector2f& position);

		void setSize(const sf::Vector2f& size);
		void setCellColor(const sf::Color& color);
		const sf::Vector2f& getPixelPosition() const;
		sf::Vector2u getPosition() const;
		sf::RectangleShape& getShape() const;

	private:
		mutable sf::RectangleShape m_shape;	// holds data about each cell

		void buildGridCell();
	};

	inline void GridCell::setSize(const sf::Vector2f& size)
	{ m_shape.setSize(size); }

	inline const sf::Vector2f& GridCell::getPixelPosition() const
	{ return m_shape.getPosition(); }

	inline sf::RectangleShape& GridCell::getShape() const
	{ return m_shape; }

	inline void GridCell::setCellColor(const sf::Color& color)
	{ m_shape.setFillColor(color); }

} // namespace sp_hash

#endif // SP_HASH_CELL_HPP