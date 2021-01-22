
#ifndef SP_HASH_PALETTE_HPP
#define SP_HASH_PALETTE_HPP

#include "Object.hpp"
#include "ThreadPool.hpp"
#include "PaletteColor.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

namespace sp_hash
{
	class Palette : public sf::Drawable, sf::Transformable, IObject
	{
	public:
		using PaletteColors = std::vector<Palette>;
		Palette(const sf::Vector2u& size = sf::Vector2u(24, 24));
		virtual ~Palette();

		void addColor(const PaletteColor& color);
		void removeColor(sf::Uint32 index);
		PaletteColor& getColor(sf::Uint32 index);
		void setColor(sf::Uint32 index);
		void selectColor(const PaletteColor& color);				// select color not in palette
		void nextColor();
		void prevColor();
		PaletteColor& getCurrentSelection();
		void addDefaultColors();
		void clearColors();
		void setAlpha(sf::Uint8 alpha);
		void setCellSize(const sf::Vector2u& size);
		void build();
		
		sf::FloatRect getColorBounds(sf::Uint32 index);
		sf::FloatRect getColorBounds(const sf::Vector2u& position);

		std::vector<PaletteColor>& getVector();
		sf::Color& getPaletteColor(sf::Uint32 index);
		sf::FloatRect getBounds();
		const sf::Vector2u& getCellSize() const;
		

		/* Transformations */
		void setPosition(const sf::Vector2u& position);
		void offsetPosition(const sf::Vector2u position);
		sf::Vector2u getPosition() const;

		/* Highlighting */
		void onSelectHighlight();
		void onHoverHighlight();

		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(float deltaTime);
		void render(sf::RenderWindow& window);
		

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
		sf::Uint32 m_currentIndex;
		PaletteColor m_currentColor;
		// ThreadPool m_threadPool;
		sf::VertexArray m_vertexArray;
		sf::Texture *m_texture;
		sf::Vector2u m_position;
		sf::Vector2u m_size;
		sf::Vector2u m_cellSize;
		std::vector<PaletteColor> m_paletteColors;

		sf::Vertex* getQuad(sf::Uint32 index);
		sf::Vertex* getQuad(const sf::Vector2u& position);
		sf::FloatRect getQuadBounds(sf::Vertex *quad);
		void unhighlightSelected();
		void setQuadAlpha(sf::Uint8 alpha);
		void setQuadAlpha(sf::Vertex *quad, sf::Uint8 alpha);
	};

	inline void Palette::addColor(const PaletteColor& color)
	{ m_paletteColors.emplace_back(color); }

	inline void Palette::removeColor(sf::Uint32 index)
	{ m_paletteColors.erase(m_paletteColors.begin() + index); }

	inline PaletteColor& Palette::getColor(sf::Uint32 index)
	{ return m_paletteColors.at(index); }

	inline PaletteColor& Palette::getCurrentSelection()
	{ return m_paletteColors.at(m_currentIndex); }

	inline void Palette::clearColors()
	{ m_paletteColors.clear(); }

	inline void Palette::setAlpha(sf::Uint8 alpha)
	{ m_currentColor.getColor().a = alpha; }

	inline sf::Color& Palette::getPaletteColor(sf::Uint32 index)
	{ return m_paletteColors.at(index).getColor(); }

	inline void Palette::onHoverHighlight()
	{ setAlpha(185); }

	inline void Palette::unhighlightSelected()
	{ setAlpha(35); }

	inline std::vector<PaletteColor>& Palette::getVector()
	{ return m_paletteColors; }

	inline sf::FloatRect Palette::getBounds()
	{ return m_vertexArray.getBounds(); }

	inline const sf::Vector2u& Palette::getCellSize() const
	{ return m_cellSize; }

	inline void Palette::setPosition(const sf::Vector2u& position)
	{ m_position = position; }

	inline void Palette::offsetPosition(const sf::Vector2u position)
	{ m_position += position; }

	inline sf::Vertex* Palette::getQuad(sf::Uint32 index)
	{ return &m_vertexArray[(index + m_size.y * m_size.x) * 4]; }

	inline sf::Vertex* Palette::getQuad(const sf::Vector2u& position)
	{ return &m_vertexArray[(position.x + position.y * m_size.x) * 4]; }

	inline sf::FloatRect Palette::getQuadBounds(sf::Vertex *quad)
	{ 
		return sf::FloatRect(
				quad[0].position.x, quad[0].position.y,
				quad[2].position.x - quad[3].position.x,
				quad[2].position.y - quad[1].position.y); 
	}

	std::ostream& operator<<(std::ostream& os, Palette& palette);
}

#endif // SP_HASH_PALETTE_HPP