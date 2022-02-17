
#pragma once

#include "object.hpp"
#include "brush.hpp"
#include "tree.hpp"
#include "input_map.hpp"
#include "canvas_layer.hpp"

#include <vector>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"


namespace ez_paint
{
	using namespace utils;
	using namespace types;
	// NOTE: A quad represents a pixel so a quad can only have a single color for
	// ALL of its vertices.

	
	class Canvas : public sf::Drawable, public sf::Transformable, public Object
	{
	public:
		Canvas(Object *parent, const sf::Vector2u& gridSize, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));
		explicit Canvas(const sf::Vector2u& gridSize, const sf::Vector2u& cellSize = sf::Vector2u(32, 32));
		~Canvas();
		
		/* Transformation Setter functions */
		void setSize(const sf::Vector2u& size);
		void setCellSize(const sf::Vector2u& size);
		void setPosition(const sf::Vector2u& position);
		void setPixel(const sf::Vector2u& position, const sf::Color& color);
		void setBrushPrimaryColor(const sf::Color& color);
		void setBrushSecondaryColor(const sf::Color& color);

		/* Getter functions */
		const sf::Vector2u& getGridSize() const;
		const sf::Vector2u& getCellSize() const;
		Palette& getCanvasColorsRef();
		sf::Vector2u getSize() const;
		const Brush& getBrush() const;
		sf::Color getPixel(const sf::Vector2u& position);


		/* Interface funtions */
		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(sf::RenderWindow& window, float deltaTime);
		void render(sf::RenderWindow& window);

		/* SpriteBatch'ing functions */
		void batch(sf::Color color = sf::Color(255, 255, 255, 255), sf::PrimitiveType type = sf::LinesStrip);
		const sf::VertexArray& getVertexArray() const;

	private:
		bool m_enableDraw;
		float m_updateTimer;
		sf::VertexArray m_vertexArray;
		sf::RectangleShape m_gridShadow;
		sf::Vector2u m_gridSize;
		sf::Vector2u m_cellSize;
		// sf::Shader *m_shadowShader;
		sf::Texture *m_texture;
		sf::Vector2u m_position;
		ImGui::FileBrowser m_imFileDialog;
		Quadtree m_quadtree;
		Brush m_brush;
		ImVec4 m_imColorEdit;
		Palette m_canvasColors;
		CanvasLayers m_canvasLayers;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void hashBrush();
		
		sf::Vertex* getQuad(sf::Uint32 index);
		sf::Vertex* getQuad(const sf::Vector2u& position);
		sf::Color getQuadColor(const sf::Vertex *quad);
		sf::FloatRect getQuadBounds(const sf::Vertex *quad);
		void setQuadAlpha(sf::Uint8 alpha);
		void setQuadAlpha(sf::Vertex *quad, sf::Uint8 alpha);
		void setQuadColor(sf::Vertex *quad, const sf::Color& color);
		void setQuadPosition(sf::Vertex *quad, sf::Vector2f position);
	};

	inline sf::Vertex* Canvas::getQuad(sf::Uint32 index)
	{ return &m_vertexArray[(index + m_gridSize.y * m_gridSize.x) * 4]; }

	inline sf::Vertex* Canvas::getQuad(const sf::Vector2u& position)
	{ return &m_vertexArray[(position.x + position.y * m_gridSize.x) * 4]; }

	inline sf::FloatRect Canvas::getQuadBounds(const sf::Vertex *quad)
	{
		return sf::FloatRect(
				quad[0].position.x, quad[0].position.y,
				quad[2].position.x - quad[3].position.x,
				quad[2].position.y - quad[1].position.y); 
	}

	inline sf::Color Canvas::getQuadColor(const sf::Vertex *quad)
	{ return sf::Color(quad[0].color); }

	inline const sf::VertexArray& Canvas::getVertexArray() const
	{ return m_vertexArray; }

	inline void Canvas::setSize(const sf::Vector2u& size)
	{ m_gridSize = size; }

	inline void Canvas::setCellSize(const sf::Vector2u& size)
	{ m_cellSize = size; }

	inline void Canvas::setPosition(const sf::Vector2u& position)
	{ m_position = position; }

	inline void Canvas::setPixel(const sf::Vector2u &position, const sf::Color &color)
	{ setQuadColor(getQuad(position), color); }

	inline void Canvas::setBrushPrimaryColor(const sf::Color &color)
	{ m_brush.setPrimaryColor(color); }

	inline void Canvas::setBrushSecondaryColor(const sf::Color &color)
	{ m_brush.setSecondaryColor(color); }

	inline const sf::Vector2u& Canvas::getGridSize() const
	{ return m_gridSize; }

	inline const sf::Vector2u& Canvas::getCellSize() const
	{ return m_cellSize; }

	inline sf::Vector2u Canvas::getSize() const
	{ return sf::Vector2u(m_gridSize.x * m_cellSize.x, m_gridSize.x * m_cellSize.y); }

	inline const Brush& Canvas::getBrush() const
	{ return m_brush; }

	inline sf::Color Canvas::getPixel(const sf::Vector2u &position)
	{ return getQuadColor(getQuad(position)); }
	
} // namespace ez_paint
