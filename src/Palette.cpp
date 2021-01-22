
#include "Palette.hpp"
#include "Utils.hpp"
#include "App.hpp"
#include <cmath>
#include <iostream>

namespace sp_hash
{
	Palette::Palette(const sf::Vector2u& size)
	: m_currentIndex(0)
	, m_currentColor(sf::Color::Black)
	, m_texture(new sf::Texture())
	, m_position(sf::Vector2u(0,0))
	, m_size(size)
	, m_cellSize(sf::Vector2u(16, 16))
	{
		addDefaultColors();
		m_currentColor = getCurrentSelection();
		setCellSize(size);
		build();
		onSelectHighlight();
	}

	Palette::~Palette()
	{
		delete m_texture;
	}

	void Palette::setColor(unsigned int index)
	{
		if (index > m_paletteColors.size() - 1) 
			index = m_paletteColors.size() - 1;
		else if (index < 0) 
			index = 0;
		m_currentIndex = index;
		onSelectHighlight();
	}

	void Palette::nextColor()
	{
		if (m_currentIndex < m_paletteColors.size() - 1) 
			++m_currentIndex;
		else 
			m_currentIndex = 0;
		onSelectHighlight();
	}

	void Palette::prevColor()
	{
		if (m_currentIndex > 0) 
			--m_currentIndex;
		else 
			m_currentIndex = m_paletteColors.size() - 1;
		onSelectHighlight();
	}

	void Palette::addDefaultColors()
	{
		m_paletteColors.clear();
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Black));
		m_paletteColors.emplace_back(PaletteColor(sf::Color(102, 102, 102, 255)));	// Gray
		m_paletteColors.emplace_back(PaletteColor(sf::Color(128, 0, 0, 255)));		// Maroon
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Red));					// Red
		m_paletteColors.emplace_back(PaletteColor(sf::Color(255, 102, 0, 255)));	// Orange
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Yellow));				// Yellow
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Green));				// Green
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Cyan));				// Cyan
		m_paletteColors.emplace_back(PaletteColor(sf::Color(0, 155, 255, 255)));	// Mid-Blue
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Blue));				// Blue
		m_paletteColors.emplace_back(PaletteColor(sf::Color(128, 0, 255, 255)));	// Purple
		m_paletteColors.emplace_back(PaletteColor(sf::Color(191, 0, 255)));			// Purple-Pink
		m_paletteColors.emplace_back(PaletteColor(sf::Color::Magenta));				// Magenta
		m_paletteColors.emplace_back(PaletteColor(sf::Color(255, 0, 191, 255)));	// Hot Pink

		foreach(m_paletteColors, [](PaletteColor& pColor){
			pColor.getColor().a = 250;
		});
	}

	void Palette::setCellSize(const sf::Vector2u& size)
	{
		m_cellSize.x = size.x;
		m_cellSize.y = size.y;
	}

	void Palette::build()
	{
		m_vertexArray.setPrimitiveType(sf::Quads);
		m_vertexArray.resize(m_size.x * m_size.y * 4);

		sf::Uint32 i = 0, j = 0;
		for (auto& pColor : m_paletteColors)
		{
			pColor.setTopLeft(sf::Vector2f(i * m_cellSize.x, j * m_cellSize.y));
			pColor.setTopRight(sf::Vector2f((i + 1) * m_cellSize.x, j * m_cellSize.y));
			pColor.setBottomRight(sf::Vector2f((i + 1) * m_cellSize.x, (j + 1) * m_cellSize.y));
			pColor.setBottomLeft(sf::Vector2f(i * m_cellSize.x, (j + 1) * m_cellSize.y));
			
			m_vertexArray.append(pColor.getTopLeft());
			m_vertexArray.append(pColor.getTopRight());
			m_vertexArray.append(pColor.getBottomRight());
			m_vertexArray.append(pColor.getBottomLeft());

			++i;
			if (i > m_size.x) 
				++j;
		}

		
		// auto size = m_paletteColors.size();
		// for(sf::Uint32 j = 0; j < m_size.y; ++j)
		// {
		// 	for(sf::Uint32 i = 0; i < m_size.x; ++i)
		// 	{
		// 		PaletteColor pColor = m_paletteColors[(i + j * m_size.x)];
		// 		pColor.setTopLeft(sf::Vector2f(i * m_cellSize.x, j * m_cellSize.y));
		// 		pColor.setTopRight(sf::Vector2f((i + 1) * m_cellSize.x, j * m_cellSize.y));
		// 		pColor.setBottomRight(sf::Vector2f((i + 1) * m_cellSize.x, (j + 1) * m_cellSize.y));
		// 		pColor.setBottomLeft(sf::Vector2f(i * m_cellSize.x, (j + 1) * m_cellSize.y));
				
		// 		m_vertexArray.append(pColor.getTopLeft());
		// 		m_vertexArray.append(pColor.getTopRight());
		// 		m_vertexArray.append(pColor.getBottomRight());
		// 		m_vertexArray.append(pColor.getBottomLeft());

		// 	}
		// }

		/* Set the new pallete width size */
		m_size.x = m_paletteColors.size();
		m_size.y = 1;

		std::cout << "palette.colors.size: " << m_paletteColors.size() << "\n";
		std::cout << "palette.position: " << m_position  << "\n";
		std::cout << "palette.size: " << m_size << "\n"; 
		std::cout << "palette.color: " << m_currentColor << "\n";
	}

	void Palette::onSelectHighlight()
	{
		unhighlightSelected();
		if (m_vertexArray.getVertexCount() > 0)
			setQuadAlpha(getQuad(m_currentIndex), 255);
	}

	sf::FloatRect Palette::getColorBounds(unsigned int index)
	{
		if (m_vertexArray.getVertexCount() > 0)
			return getQuadBounds(getQuad(index));
		return sf::FloatRect();
	}

	sf::FloatRect Palette::getColorBounds(const sf::Vector2u& position)
	{
		if (m_vertexArray.getVertexCount() > 0)
			return getQuadBounds(getQuad(position));
		return sf::FloatRect();
	}

	void Palette::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertexArray, states);
	}

	void Palette::setQuadAlpha(uint8_t alpha)
	{
		if (m_vertexArray.getVertexCount() > 0)
			for (unsigned int i = 0; i < m_paletteColors.size(); ++i)
				setQuadAlpha(getQuad(i), alpha);
	}

	void Palette::setQuadAlpha(sf::Vertex *quad, uint8_t alpha)
	{
		quad[0].color.a = alpha;
		quad[1].color.a = alpha;
		quad[2].color.a = alpha;
		quad[3].color.a = alpha;
	}

	void Palette::handleEvents(sf::RenderWindow& window, sf::Event& event)
	{
		/* Highlight color on mouse hover */
		if (getBounds().contains(App::mousePosition))
		{
			onHoverHighlight();						
			window.setMouseCursorVisible(true);
		}
		else
		{
			onSelectHighlight();
			window.setMouseCursorVisible(false);
		}

		/* Set color from mouse click */
		if(event.type == sf::Event::MouseButtonPressed)
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if(getBounds().contains(App::mousePosition))
				{
					auto size = getVector().size();
					for(sf::Uint32 i = 0; i < size; ++i)
						if(getColorBounds(i).contains(App::mousePosition))
							setColor(i);
				}
			}
		}

		/* Change color with keyboard */
		if(event.type == sf::Event::KeyPressed)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				nextColor();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				prevColor();
		}
		
	}

	void Palette::update(float deltaTime)
	{

	}

	void Palette::render(sf::RenderWindow& window)
	{
		// printDebug("call palette render");
		// window.draw(*this);
		window.draw(m_vertexArray);
	}

	std::ostream& operator<<(std::ostream& os, Palette& palette)
    { return os << "<" << ">"; }
} // namespace sp_hash

