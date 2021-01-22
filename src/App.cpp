// spatial_hash_test.cpp : Defines the entry point for the console application.
//

#include "Object.hpp"
#include "ObjectManager.hpp"
#include "Grid.hpp"
#include "GridCell.hpp"
#include "Brush.hpp"
#include "SpatialHash.hpp"
#include "SpriteBatch.hpp"
#include "Palette.hpp"
#include "Utils.hpp"
#include "App.hpp"

// Check if using X11
#ifdef X_PROTOCOL
#include <X11/Xlib.h>
#endif

namespace sp_hash
{
	namespace App
	{
		sf::String WindowTitle 				= "Pixel Edit";
		sf::Vector2f WindowSize				= sf::Vector2f(800, 600);
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
	sf::RenderWindow window(sf::VideoMode(App::WindowSize.x, App::WindowSize.y, App::WindowBpp), App::WindowTitle);
	window.setFramerateLimit(App::FrameLimit);
	window.setMouseCursorVisible(false);
	sf::Event event;

	/* Create an object manager to handle object loops */
	ObjectManager manager(window, event);
	Grid *grid = (Grid*)manager.createObject(new Grid(App::GridSize, App::GridCellSize));

	// /* Create a 16 x 16 grid */
	// Grid grid(App::GridSize); // in cells
	// grid.setCellSize(App::GridCellSize);

	// /* Set app window size to grid size */
	App::WindowSize = sf::Vector2f(grid->getPixelSize().x, grid->getPixelSize().y);
	Brush *brush = (Brush*)manager.createObject(new Brush(20, App::WindowSize));
	grid->setBrush(brush);

	// /* Create brush object that follows mouse pointer */
	// Object brush(20); // in px
	// brush.setPosition(App::WindowSize);
	// brush.setOriginToCenter();

	// /* Create spatial hash with for grid */
	SpatialHash table(*grid);

	// /* Batch the grid into a vertex array */
	// SpriteBatch lineBatch = SpriteBatch(grid);
	SpriteBatch quadBatch = SpriteBatch(*grid);
	// lineBatch.batch(App::GridLineColor);
	quadBatch.batch(App::GridFillColor, sf::Quads);

	

	// /* Loop control */
	// bool isRunning = true;

	// while (isRunning)
	// {
	// 	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	// 	bool moving = false;
	// 	float zoom = 1.0f;
	// 	sf::Vector2f lastMousePosition;
	// 	sf::Event event;
	// 	sf::View cameraView;

	// 	lineBatch.batch(App::GridLineColor);

	// 	/* Handle events if window has focus */
	// 	sf::View view = window.getDefaultView();
		// while (window.pollEvent(event) && window.hasFocus())
		// {
		// 	/* Close Button Event */
		// 	if (event.type == sf::Event::Closed)
		// 		isRunning = false;

		// 	if (event.type == sf::Event::Resized){}
		// 		//window.setView(calcView(WINDOW_SIZE, 1.f, 1.f));

		// 	/* Mouse Wheel Events */
		// 	if (event.type == sf::Event::MouseWheelScrolled)
		// 	{
		// 		if (event.mouseWheelScroll.delta > 0 && brush.getRadius() < App::BrushSizeMax)
		// 			brush.changeSize(App::BrushSizeDelta);
		// 		else if (event.mouseWheelScroll.delta < 0 && brush.getRadius() > App::BrushSizeMin)
		// 			brush.changeSize(-App::BrushSizeDelta);
					
		// 		/* Check if brush size is within threshold after mouse wheel event */
		// 		if (brush.getRadius() < App::BrushSizeMin)
		// 			brush.setRadius(App::BrushSizeMin);

		// 		if (brush.getRadius() > App::BrushSizeMax)
		// 			brush.setRadius(App::BrushSizeMax);
				
		// 		/* Update the brush's origin after resize */
		// 		brush.setOriginToCenter();
		// 		std::cout << "brush.size: " << brush.getRadius() << "\n";
		// 	}

	// 		/*if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	// 		{
	// 			if (event.mouseWheelScroll.delta > 0)
	// 				zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.f));
	// 			else if (event.mouseWheelScroll.delta < 0)
	// 				zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.f);
	// 		}*/

	// 		switch(event.type)
	// 		{
	// 		case sf::Event::MouseButtonPressed:
	// 		{
	// 			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	// 				if (event.mouseButton.button == 0)
	// 				{
	// 					moving = true;
	// 					lastMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
	// 					break;
	// 				}
	// 		}
	// 		case sf::Event::MouseButtonReleased:
	// 		{
	// 			moving = !(event.mouseButton.button == 0);
	// 			break;
	// 		}
	// 		case sf::Event::MouseMoved:
	// 		{
	// 			if (!moving)
	// 				break;
	// 			const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	// 			const sf::Vector2f deltaPos = lastMousePosition - newPos;

	// 			// Move our view accordingly and update the window
	// 			view.setCenter(view.getCenter() + deltaPos);
	// 			window.setView(view);

	// 			// Save the new position as the old one
	// 			// We're recalculating this, since we've changed the view
	// 			lastMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	// 			break;
	// 		}
	// 		case sf::Event::MouseWheelScrolled:
	// 		{
	// 			if (moving)
	// 				break;

	// 			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	// 			{
	// 				// Determine the scroll direction and adjust the zoom level
	// 				// Again, you can swap these to invert the direction
	// 				if (event.mouseWheelScroll.delta > 0)
	// 					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.f));
	// 				else if (event.mouseWheelScroll.delta < 0)
	// 					zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.f);

	// 				// Update our view
	// 				view.setSize(window.getDefaultView().getSize()); // Reset the size
	// 				view.zoom(zoom); // Apply the zoom level (this transforms the view)
	// 				window.setView(view);
	// 				break;
	// 			}
				
	// 		}
	// 		} // switch

	// 		if (event.type == sf::Event::MouseButtonPressed)
	// 			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	// 				if (grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 					for (int i = 0; i < grid.getPaletteRef().getPaletteVector().size(); ++i)
	// 						if (grid.getPaletteRef().getColorBounds(i).contains(mousePosition)) 
	// 							grid.getPaletteRef().setColor(i);

	// 		/* Change color events */
	// 		if (event.type == sf::Event::KeyPressed)
	// 		{
	// 			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	// 				grid.getPaletteRef().nextColor();
	// 			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	// 				grid.getPaletteRef().prevColor();
	// 		}
	// 	} // Handle Events

		
		
	// 	// If mouse hovers over palette
	// 	if (window.hasFocus())
	// 	{
	// 		if (grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 		{
	// 			grid.getPaletteRef().onHoverHighlight();						// Change opacity of palette to more visible
	// 			window.setMouseCursorVisible(true);
	// 		}
	// 		else
	// 		{
	// 			grid.getPaletteRef().onSelectHighlight();
	// 			window.setMouseCursorVisible(false);
	// 		}

	// 		/* Mouse Button Events */
	// 		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	// 		{
	// 			// If not clicking on palette
	// 			if (!grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 			{
	// 				window.setMouseCursorVisible(false);
	// 				brush.setTrigger(true);
	// 				brush.setFillColor(sf::Color(App::BrushColorOnClickLeft));
	// 				table.hash(brush, quadBatch);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			brush.setFillColor(App::BrushColor);
	// 		}

	// 		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	// 		{
	// 			if (!grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 			{
	// 				brush.setTrigger(false);
	// 				brush.setFillColor(sf::Color(App::BrushColorOnClickRight));
	// 				table.hash(brush, quadBatch);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			brush.setFillColor(App::BrushColor);
	// 		}

	// 		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	// 		{
	// 			if (grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 				grid.setPalettePosition(static_cast<sf::Vector2u>(mousePosition));

	// 			if (quadBatch.getVertexArray().getBounds().contains(mousePosition))
	// 			{
	// 				//window.getView().move(mousePosition);
	// 			}
	// 		}
	// 	}
		
	// 	/* Update Logic */
	// 	brush.setPosition(mousePosition);	

	// 	/* Draw Grid, Cells, and Objects */
	// 	window.clear(sf::Color(200, 200, 200));

	// 	/* Draw each cell in grid */
	// 	for (const auto& row : grid.getGridVector())
	// 		for (auto cell : row)
	// 			window.draw(cell.getShape());
				
	// 	/* Draw brush's shape */
	// 	window.draw(quadBatch);
	// 	// window.draw(lineBatch);
	// 	if (!grid.getPaletteRef().getPaletteBounds().contains(mousePosition))
	// 		window.draw(brush.getShape());
	// 	window.draw(grid.getPaletteRef());
	// 	window.display();
	// 	table.clear();
		
	// 	//if (t.joinable())
	// 		//t.join();
		
	// } // loop

	manager.loop();
	return 0;
}

