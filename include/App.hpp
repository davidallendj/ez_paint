#ifndef SP_HASH_APP_HPP
#define SP_HASH_APP_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


/* Set App properties */
namespace sp_hash
{
	namespace App
	{
		constexpr float FrameLimit					= 60;
		constexpr int BrushSizeMax					= 100;
		constexpr int BrushSizeMin					= 10;
		constexpr int BrushSizeDelta				= 10;
		const auto BrushColor						= sf::Color(0, 0, 255, 100);
		const auto BrushColorOnClickLeft			= sf::Color(255, 255, 0, 100);
		const auto BrushColorOnClickRight			= sf::Color(255, 0, 0, 100);
		const auto GridSize							= sf::Vector2u(64, 64);
		const auto GridLineColor					= sf::Color(255, 0, 0, 200);
		const auto GridFillColor					= sf::Color(255, 255, 255, 250);
		const auto GridCellSize						= sf::Vector2u(32, 32);
		const auto FixedUpdateTimeStep				= 16.6;
		extern sf::String WindowTitle;
		extern sf::Vector2f WindowSize;
		extern sf::Uint32 WindowBpp;
		extern sf::Vector2f mousePosition;
		extern sf::Vector2f lastMousePosition;
	}
}


#endif // SP_HASH_APP_HPP