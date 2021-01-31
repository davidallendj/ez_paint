
#include "SpriteBatch.hpp"
#include "Grid.hpp"

namespace sp_hash
{
    SpriteBatch::SpriteBatch(Grid& grid)
    : m_grid(grid)
    , m_texture(nullptr)
    , m_size(sf::Vector2u(0, 0))
    , m_bound(sf::FloatRect(0, 0, 0, 0))
    { build(); }

    void SpriteBatch::batch(sf::Color color, sf::PrimitiveType type)
    {
        sf::Vector2u cellSize = m_grid.getCellSize();
        m_vertexArray.setPrimitiveType(type);

        /* Populate the vertex array, with one quad per cell */
        for(sf::Uint32 j = 0; j < m_size.y; ++j)
        {
            for(sf::Uint32 i = 0; i < m_size.x; ++i)
            {
                sf::Vertex *vertex = &m_vertexArray[(i + j * m_size.x) * 4];
                if (type == sf::LinesStrip)
                {
                    vertex[0].position = sf::Vector2f(i * cellSize.x, j * cellSize.y);
                    vertex[1].position = sf::Vector2f((i + 1) * cellSize.x, j * cellSize.y);
                    vertex[2].position = sf::Vector2f((i + 1) * cellSize.x, (j + 1) * cellSize.y);
                    vertex[3].position = sf::Vector2f(i * cellSize.x, (j + 1) * cellSize.y);
                }
                else
                {
                    // Define its 4 corners
                    vertex[0].position = sf::Vector2f(i * cellSize.x, j * cellSize.y);
                    vertex[1].position = sf::Vector2f((i + 1) * cellSize.x, j * cellSize.y);
                    vertex[2].position = sf::Vector2f((i + 1) * cellSize.x, (j + 1) * cellSize.y);
                    vertex[3].position = sf::Vector2f(i * cellSize.x, (j + 1) * cellSize.y);
                }
                vertex[0].color = color;
                vertex[1].color = color;
                vertex[2].color = color;
                vertex[3].color = color;
            }
        }
    }

    const sf::FloatRect& SpriteBatch::getCellBounds(const sf::Vector2u& position)
    {
        // Get pointer to cell quad
        if (m_vertexArray.getVertexCount() > 0)
        {
            sf::Vertex* quad = &m_vertexArray[(position.x + position.y * m_size.x) * 4];
            m_bound = sf::FloatRect(
                quad[0].position.x, quad[0].position.y,
                quad[2].position.x - quad[3].position.x,
                quad[2].position.y - quad[1].position.y);
            return m_bound;
        }
        return m_bound;
    }

    // sf::FloatRect* SpriteBatch::getCellBoundsPtr(const sf::Vector2u& position)
    // {
    //     if()
    // }

    sf::RectangleShape SpriteBatch::getCellShape(const sf::Vector2u& position)
    {
        sf::RectangleShape shape;
        // Get pointer to cell quad
        if (m_vertexArray.getVertexCount() > 0)
        {
            sf::Vertex* quad = &m_vertexArray[(position.x + position.y * m_size.x) * 4];
            shape.setPosition(sf::Vector2f(quad[0].position.x, quad[0].position.y));
            shape.setSize(sf::Vector2f(
                quad[2].position.x - quad[3].position.x,
                quad[2].position.y - quad[1].position.y));
            return shape;
        }
        return shape;
    }

    void SpriteBatch::setCellColor(const sf::Vector2u& position, const sf::Color& color)
    {
        if (m_vertexArray.getVertexCount() > 0)
        {
            sf::Vertex* quad = &m_vertexArray[(position.y + position.x * m_size.x) * 4];
            quad[0].color = color;
            quad[1].color = color;
            quad[2].color = color;
            quad[3].color = color;
        }
    }

    void SpriteBatch::build()
    {
        m_size.x = m_grid.getSize().x;
        m_size.y = m_grid.getSize().y;
        m_vertexArray.resize(m_size.x * m_size.y * 4);
    }

    void SpriteBatch::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        // states.texture = m_texture;
        target.draw(m_vertexArray, states);
    }
}