// spatial_hash_test.cpp : Defines the entry point for the console application.
//

#include "Object.hpp"
#include "App.hpp"
#include "Grid.hpp"
#include "GridCell.hpp"
#include "Brush.hpp"
#include "SpriteBatch.hpp"
#include "Utils.hpp"
#include "AppConfig.hpp"

// Check if using X11
#ifdef X_PROTOCOL
#include <X11/Xlib.h>
#endif

namespace sp_hash
{
	namespace AppConfig
	{
		sf::String WindowTitle 				= "Pixel Edit";
		sf::Vector2u WindowSize				= sf::Vector2u(800, 600);
		sf::Uint32 WindowBpp				= 32;
		sf::Vector2f mousePosition			= sf::Vector2f(0, 0);
		sf::Vector2f lastMousePosition		= sf::Vector2f(0, 0);
	}
}

//ez_paint.exe
int main(int argc, char* argv[])
{
	using namespace sp_hash;

#ifdef X_PROTOCOL
	XInitThreads();
#endif

	/* Create app window */
	sf::RenderWindow window(sf::VideoMode(AppConfig::WindowSize.x, AppConfig::WindowSize.y, AppConfig::WindowBpp), AppConfig::WindowTitle);
	window.setFramerateLimit(AppConfig::FrameLimit);
	window.setMouseCursorVisible(false);

	/* Create an object app to handle object loops */
	App app(window);
	Grid *grid = (Grid*)app.createObject(new Grid(AppConfig::GridSize, AppConfig::GridCellSize));

	// /* Set app window size to grid size */
	AppConfig::WindowSize = sf::Vector2u(
		grid->getPixelSize().x, 
		grid->getPixelSize().y
	);
	app.loop();

	return 0;
}

