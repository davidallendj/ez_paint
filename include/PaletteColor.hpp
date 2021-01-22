
#ifndef SP_HASH_PALETTE_COLOR_HPP
#define SP_HASH_PALETTE_COLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <ostream>


namespace sp_hash
{
	class PaletteColor
	{
	public:
		explicit PaletteColor(const sf::Color& color);

		/* Getters */
		sf::Color& getColor();
		sf::Vertex& getTopLeft();
		sf::Vertex& getTopRight();
		sf::Vertex& getBottomRight();
		sf::Vertex& getBottomLeft();
		sf::VertexArray getVertexArray() const;

		/* Setters */
		void setColor(const sf::Color& color);
		void setTopLeft(const sf::Vector2f position);
		void setTopRight(const sf::Vector2f position);
		void setBottomRight(const sf::Vector2f position);
		void setBottomLeft(const sf::Vector2f position);


	private:
		sf::Color m_color;
		sf::Vertex m_topLeft;
		sf::Vertex m_topRight;
		sf::Vertex m_bottomLeft;
		sf::Vertex m_bottomRight;
	};
	
	inline sf::Color& PaletteColor::getColor()
    { return m_color; }

    inline sf::Vertex& PaletteColor::getTopLeft()
    { return m_topLeft; }

    inline sf::Vertex& PaletteColor::getTopRight()
    { return m_topRight; }

    inline sf::Vertex& PaletteColor::getBottomLeft()
    { return m_bottomLeft; }

    inline sf::Vertex& PaletteColor::getBottomRight()
    { return m_bottomRight; }

    inline void PaletteColor::setColor(const sf::Color& color)
    { m_color = color; }

    inline void PaletteColor::setTopLeft(const sf::Vector2f position)
    { m_topLeft.position = position; }

    inline void PaletteColor::setTopRight(const sf::Vector2f position)
    { m_topRight.position = position; }

    inline void PaletteColor::setBottomLeft(const sf::Vector2f position)
    { m_bottomLeft.position = position; }
	
	inline void PaletteColor::setBottomRight(const sf::Vector2f position)
    { m_bottomRight.position = position; }

	inline sf::VertexArray PaletteColor::getVertexArray() const
	{
		sf::VertexArray vertexArray(sf::Quads, 4);
		vertexArray.append(m_topLeft);
		vertexArray.append(m_topRight);
		vertexArray.append(m_bottomLeft);
		vertexArray.append(m_bottomRight);
		return vertexArray;
	}

	std::ostream& operator<<(std::ostream& os, PaletteColor& palette);

}

#endif // SP_HASH_PALETTE_COLOR_HPP