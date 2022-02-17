#pragma once

#include "utils.hpp"
#include "log.hpp"
#include "format.hpp"
#include "types.hpp"

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


/* Set App properties */
namespace ez_paint::config
{
	using namespace utils;

	extern float FramesPerSecond;
	extern sf::Uint32 TreePartitions;
	extern types::rangef_t CameraZoom;
	extern types::rangef_t BrushSize;
	extern sf::Uint32 BrushSizeDelta;
	extern sf::Color BrushDefaultColor;
	extern std::string WindowTitle;
	extern sf::Vector2u WindowSize;
	extern sf::Uint32 WindowBpp;
	extern sf::Uint32 ThreadCount;
	extern sf::Vector2f MousePosition;
	extern sf::Vector2f LastMousePosition;
	extern sf::Vector2u CanvasSize;
	extern sf::Vector2u CanvasCellSize;
	extern bool CanvasLines;
	extern bool VerticalSyncEnabled;
	extern sf::Color BackgroundColor;

	void showConfig();

}
