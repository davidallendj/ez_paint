
#ifndef SP_HASH_GRID_HPP
#define SP_HASH_GRID_HPP

#include "GridCell.hpp"
#include "Quadtree.hpp"
#include "Object.hpp"
#include "Brush.hpp"
#include "InputMap.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>

#include "imgui.h"
#include "imgui-SFML.h"

namespace sp_hash
{
	class Brush;
	class SpriteBatch;
	class SpatialHash;
	class Grid : public sf::Drawable, public sf::Transformable, public Object
	{
	public:
		typedef std::vector<std::vector<GridCell>> GridVector;
		typedef std::vector<sf::Vertex*> GridBrushBounds;
		typedef std::vector<sf::Color> GridColors;

		Grid(Object *parent, const sf::Vector2u& gridSize, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));
		explicit Grid(const sf::Vector2u& gridSize, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));
		~Grid();
		
		/* Transformation Setter functions */
		void setSize(const sf::Vector2u& size);
		void setCellSize(const sf::Vector2u& size);
		void setPosition(const sf::Vector2u& position);

		/* Getter functions */
		SpriteBatch* getSpriteBatchPtr();
		const sf::Vector2u& getSize() const;
		const sf::Vector2u& getCellSize() const;
		sf::Vector2u getPixelSize() const;
		const GridVector& getGridVector() const;

		/* Interface funtions */
		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(float deltaTime);
		void render(sf::RenderWindow& window);

		/* SpriteBatch'ing functions */
		void batch(sf::Color color = sf::Color(255, 255, 255, 255), sf::PrimitiveType type = sf::LinesStrip);
		const sf::VertexArray& getVertexArray() const;

	private:
		GridVector m_grid;
		GridColors m_paletteColors;
		// InputMap<> m_inputMap;
		sf::VertexArray m_vertexArray;
		sf::RectangleShape m_gridShadow;
		sf::Texture *m_texture;
		sf::Vector2u m_gridSize;
		sf::Vector2u m_cellSize;
		sf::Vector2u m_position;
		// SpatialHash *m_spatialHash;
		Quadtree m_quadtree;
		Brush m_brush;
		ImVec4 m_colorEdit;
		sf::Shader *m_shadowShader;
		bool m_enableDraw;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void hashBrush();
		
		sf::Vertex* getQuad(sf::Uint32 index);
		sf::Vertex* getQuad(const sf::Vector2u& position);
		sf::FloatRect getQuadBounds(sf::Vertex *quad);
		void setQuadAlpha(sf::Uint8 alpha);
		void setQuadAlpha(sf::Vertex *quad, sf::Uint8 alpha);
		void setQuadColor(sf::Vertex *quad, const sf::Color& color);
		void setQuadPosition(sf::Vertex *quad, sf::Vector2f position);
	};

	inline sf::Vertex* Grid::getQuad(sf::Uint32 index)
	{ return &m_vertexArray[(index + m_gridSize.y * m_gridSize.x) * 4]; }

	inline sf::Vertex* Grid::getQuad(const sf::Vector2u& position)
	{ return &m_vertexArray[(position.x + position.y * m_gridSize.x) * 4]; }

	inline sf::FloatRect Grid::getQuadBounds(sf::Vertex *quad)
	{
		return sf::FloatRect(
				quad[0].position.x, quad[0].position.y,
				quad[2].position.x - quad[3].position.x,
				quad[2].position.y - quad[1].position.y); 
	}

	inline const sf::VertexArray& Grid::getVertexArray() const
	{ return m_vertexArray; }

	inline void Grid::setSize(const sf::Vector2u& size)
	{ m_gridSize = size; }

	inline void Grid::setCellSize(const sf::Vector2u& size)
	{ m_cellSize = size; }

	inline void Grid::setPosition(const sf::Vector2u& position)
	{ m_position = position; }

	inline const sf::Vector2u& Grid::getSize() const
	{ return m_gridSize; }

	inline const sf::Vector2u& Grid::getCellSize() const
	{ return m_cellSize; }

	inline sf::Vector2u Grid::getPixelSize() const
	{ return sf::Vector2u(m_gridSize.x * m_cellSize.x, m_gridSize.x * m_cellSize.y); }

	inline const Grid::GridVector& Grid::getGridVector() const
	{ return m_grid; }
	
} // namespace sp_hash


#endif // SP_HASH_GRID_H