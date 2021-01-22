//	Ref:		 http://www.sfml-dev.org/tutorials/2.4/graphics-vertex-array.php

#ifndef SP_HASH_SPRITE_BATCH_HPP
#define SP_HASH_SPRITE_BATCH_HPP

#include "Grid.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


namespace sp_hash
{
	class SpriteBatch : public sf::Drawable, public sf::Transformable
	{
	public:
		explicit SpriteBatch(Grid& grid); 

		void batch(sf::Color color = sf::Color(150, 150, 150, 10), sf::PrimitiveType type = sf::LinesStrip);
		void setGrid(Grid& grid);

		const sf::FloatRect& getCellBounds(const sf::Vector2u& position);
		sf::RectangleShape getCellShape(const sf::Vector2u& position);
		void setCellColor(const sf::Vector2u& position, const sf::Color& color);
		void setPrimitiveType(const sf::PrimitiveType& type);
		const sf::VertexArray& getVertexArray() const;

	private:
		sf::VertexArray m_vertexArray;
		sf::Texture *m_texture;
		Grid& m_grid;
		sf::Vector2u m_size;
		sf::FloatRect m_bound;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void build();
	};

	inline void SpriteBatch::setGrid(Grid& grid)
	{ m_grid = grid; }

	inline void SpriteBatch::setPrimitiveType(const sf::PrimitiveType& type)
	{ m_vertexArray.setPrimitiveType(type); }

	inline const sf::VertexArray& SpriteBatch::getVertexArray() const
	{ return m_vertexArray; }

}


#endif // SP_HASH_SPRITE_BATCH_HPP