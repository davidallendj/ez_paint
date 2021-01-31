#ifndef SP_HASH_APP_CONFIG_HPP
#define SP_HASH_APP_CONFIG_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


/* Set App properties */
namespace sp_hash
{
	namespace AppConfig
	{
		constexpr size_t QuadtreePartitions			= 8;
		constexpr float FrameLimit					= 60;
		constexpr float CameraZoomMax				= 2.0;
		constexpr float CameraZoomMin				= -0.5;
		constexpr int BrushSizeMax					= 100;
		constexpr int BrushSizeMin					= 10;
		constexpr int BrushSizeDelta				= 10;
		const sf::Color BrushDefaultColor			= sf::Color(0, 0, 255, 100);
		const sf::Vector2u GridSize					= sf::Vector2u(256, 256);
		const sf::Vector2u GridCellSize				= sf::Vector2u(4, 4);
		const sf::Time TimePerFrame					= sf::seconds(1.f/60.f);
		extern sf::String WindowTitle;
		extern sf::Vector2u WindowSize;
		extern sf::Uint32 WindowBpp;
		extern sf::Vector2f mousePosition;
		extern sf::Vector2f lastMousePosition;
	}
}


#endif // SP_HASH_APP_CONFIG_HPP