
#include "PaletteColor.hpp"


namespace sp_hash
{
    PaletteColor::PaletteColor(const sf::Color& color) 
      : m_color(color)
      , m_topLeft(sf::Vector2f(0, 0), color)
      , m_topRight(sf::Vector2f(0, 0), color)
      , m_bottomLeft(sf:: Vector2f(0, 0), color)
      , m_bottomRight(sf::Vector2f(0, 0), color)
      { }

    std::ostream& operator<<(std::ostream& os, PaletteColor& palette)
    {
      sf::Color c = palette.getColor();
      return os << "<PaletteColor: #" 
              << std::hex << c.toInteger()
              << ">";
              // << std::hex << static_cast<sf::Uint8>(c.r) << ", " 
              // << static_cast<sf::Uint8>(c.g) << ", " 
              // << static_cast<sf::Uint8>(c.b) << ")>"; 
	  }
}
