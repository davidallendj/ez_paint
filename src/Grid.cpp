
#include "Grid.hpp"
#include "App.hpp"
#include "Brush.hpp"
#include "SpriteBatch.hpp"
#include "SpatialHash.hpp"

namespace sp_hash
{
    Grid::Grid(const sf::Vector2u& gridSize, const sf::Vector2u& cellSize)
	: m_gridSize(gridSize)
	, m_cellSize(cellSize)
	, m_position(sf::Vector2u(0, 0))
	, m_lineBatch(new SpriteBatch(*this))
	, m_spriteBatch(new SpriteBatch(*this))
	, m_spatialHash(new SpatialHash(*this))
	, m_brush(nullptr)
	, m_enableGridlines(true)
	{}

	Grid::~Grid()
	{
		delete m_lineBatch;
		delete m_spriteBatch;
		delete m_spatialHash;
	}

	void Grid::handleEvents(sf::RenderWindow& window, sf::Event& event)
	{
		if(event.type == sf::Event::MouseButtonPressed)
		{
			if(m_brush && !m_palette.getBounds().contains(App::mousePosition))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// If not clicking on palette
					window.setMouseCursorVisible(false);
					m_brush->setTrigger(true);
					m_brush->setFillColor(sf::Color(App::BrushColorOnClickLeft));
					m_spatialHash->hash(*m_brush, *m_spriteBatch);
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					m_brush->setTrigger(false);
					m_brush->setFillColor(sf::Color(App::BrushColorOnClickRight));
					m_spatialHash->hash(*m_brush, *m_spriteBatch);
				}
			}
		}
		else if(event.type == sf::Event::MouseButtonReleased)
		{
			m_brush->setFillColor(App::BrushColor);
		}
		
		

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			if (m_palette.getBounds().contains(App::mousePosition))
				setPalettePosition(static_cast<sf::Vector2u>(App::mousePosition));

			if (m_spriteBatch->getVertexArray().getBounds().contains(App::mousePosition))
			{
				//window.getView().move(App::mousePosition);
			}
		}

		if(event.type == sf::Event::MouseMoved)
		{
			// const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			// const sf::Vector2f deltaPos = App::lastMousePosition - newPos;

			// Move view accordingly and update the window
		}
		m_palette.handleEvents(window, event);
	}

	void Grid::update(float deltaTime)
	{
		m_lineBatch->batch(App::GridLineColor, sf::LinesStrip);
		m_spriteBatch->batch(App::GridFillColor, sf::Quads);
		m_palette.update(deltaTime);
	}

	void Grid::render(sf::RenderWindow& window)
	{
		window.draw(*m_spriteBatch);
		// window.draw(*m_lineBatch);
		window.draw(m_text);
		m_palette.render(window);
	}


}