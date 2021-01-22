
#ifndef SP_HASH_GRID_HPP
#define SP_HASH_GRID_HPP

#include "GridCell.hpp"
#include "Palette.hpp"
#include "Object.hpp"
// #include "Brush.hpp"
// #include "SpriteBatch.hpp"
// #include "SpatialHash.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>

namespace sp_hash
{
	class Brush;
	class SpriteBatch;
	class SpatialHash;
	class Grid : public IObject
	{
	public:
		typedef std::vector<std::vector<GridCell>> GridVector;
		explicit Grid(const sf::Vector2u& gridSize, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));
		~Grid();
		
		/* Transformation Setter functions */
		void setBrush(Brush *brush);
		void setSize(const sf::Vector2u& size);
		void setCellSize(const sf::Vector2u& size);
		void setPosition(const sf::Vector2u& position);
		void setPalettePosition(const sf::Vector2u& position);
		void offsetPalettePosition(const sf::Vector2u& position);

		/* Getter functions */
		const sf::Vector2u& getSize() const;
		const sf::Vector2u& getCellSize() const;
		sf::Vector2u getPixelSize() const;
		Palette& getPaletteRef();
		const GridVector& getGridVector() const;

		/* Interface funtions */
		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(float deltaTime);
		void render(sf::RenderWindow& window);

	private:
		GridVector m_grid;
		sf::Vector2u m_gridSize;
		sf::Vector2u m_cellSize;
		sf::Vector2u m_position;
		sf::Font m_font;
		sf::Text m_text;
		Palette m_palette;
		SpriteBatch *m_spriteBatch;
		SpriteBatch *m_lineBatch;
		SpatialHash *m_spatialHash;
		Brush *m_brush;

		// Other options
		bool m_enableGridlines;
	};

	inline void Grid::setSize(const sf::Vector2u& size)
	{ m_gridSize = size; }

	inline void Grid::setCellSize(const sf::Vector2u& size)
	{ m_cellSize = size; }

	inline void Grid::setPosition(const sf::Vector2u& position)
	{ m_position = position; }

	inline void Grid::setPalettePosition(const sf::Vector2u& position)
	{ m_palette.setPosition(position); }

	inline void Grid::offsetPalettePosition(const sf::Vector2u& position)
	{ m_palette.offsetPosition(position); }

	inline const sf::Vector2u& Grid::getSize() const
	{ return m_gridSize; }

	inline const sf::Vector2u& Grid::getCellSize() const
	{ return m_cellSize; }

	inline Palette& Grid::getPaletteRef()
	{ return m_palette; }

	inline sf::Vector2u Grid::getPixelSize() const
	{ return sf::Vector2u(m_gridSize.x * m_cellSize.x, m_gridSize.x * m_cellSize.y); }

	inline const Grid::GridVector& Grid::getGridVector() const
	{ return m_grid; }

	inline void Grid::setBrush(Brush *brush)
	{ m_brush = brush; }

	
} // namespace sp_hash


#endif // SP_HASH_GRID_H