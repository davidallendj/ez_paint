// spatial_hash_test.cpp : Defines the entry point for the console application.
//

#include "app.hpp"
#include "canvas.hpp"
#include "brush.hpp"
#include "image.hpp"
#include "types.hpp"

#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <tuple>

#include <SFML/Graphics/RenderWindow.hpp>
#include <cxxopts.hpp>

// Check if using X11
#ifdef X_PROTOCOL
	#include <X11/Xlib.h>
#endif

#ifndef BACKWARD_HAS_BFD
	#define BACKWARD_HAS_BFD 1
#endif
#ifndef BACKWARD_HAS_DW
	#define BACKWARD_HAS_DW 1
#endif
#ifndef BACKWARD_HAS_DWARF
	#define BACKWARD_HAS_DWARF 1
#endif



namespace ez_paint
{
	using namespace utils;

	namespace ui
	{
		float imColorEdit					= 0.0f;
		Brush brush							= Brush();
		Palette palette						= Palette(PaletteColors);
	}

	namespace config
	{
		float FramesPerSecond				= 60.0f;
		sf::Uint32 TreePartitions			= 8;
		types::rangef_t CameraZoom			= range(2.0, -0.5);
		types::rangef_t BrushSize			= range(5.0, 100.0);
		sf::Uint32 BrushSizeDelta			= 5;
		sf::Color BrushColor				= sf::Color(0, 0, 255, 100);
		std::string WindowTitle 			= "FUzzy PIXel EDitor";
		sf::Vector2u WindowSize				= sf::Vector2u(800, 600);
		sf::Uint32 WindowBpp				= 32;
		sf::Uint32 ThreadCount				= 4;
		sf::Vector2f MousePosition			= sf::Vector2f(0, 0);
		sf::Vector2f LastMousePosition		= sf::Vector2f(0, 0);
		sf::Vector2u CanvasSize				= sf::Vector2u(64, 64);
		sf::Vector2u CanvasCellSize			= sf::Vector2u(16, 16);
		bool VerticalSyncEnabled			= false;
		bool CanvasLines					= false;
		sf::Color CanvasLinesColors			= sf::Color(150, 150, 150, 100);
		sf::Color BackgroundColor			= sf::Color(200, 200, 200);
	}

	namespace config
	{
		void showConfig(){
			using namespace format;
			log::info(
				"Configuration:\n"
				"window title:              {}\n"
				"window size:               {}\n"
				"window bits per pixel:     {}\n"
				"fps limit:                 {}\n"
				"brush size:                {}\n"
				"threads:                   {}\n"
				"canvas total size:         {}\n"
				"canvas cell size:          {}\n",
				config::WindowTitle,
				to_string(config::WindowSize),
				config::WindowBpp,
				config::FramesPerSecond,
				to_string(config::BrushSize),
				config::ThreadCount,
				to_string(config::CanvasSize),
				to_string(config::CanvasCellSize)
			);
		}
	}

	App::App(int argc, char **argv)
	: m_isRunning(true)
	, m_threadPool(4)
	, m_window(sf::VideoMode(config::WindowSize.x, config::WindowSize.y, config::WindowBpp), config::WindowTitle)
	, m_event()
	, m_clock(sf::Clock())
	, m_time(sf::Time())
	, m_dragPosition(sf::Vector2f())
	, m_zoom(1.0)
	, m_zoomSensitivity(0.10)
	, m_cameraSensitivity(0.30)
	{
		log::info("Initializing application.");
		parseArgs(argc, argv);
		ImGui::SFML::Init(m_window);
		
		// Set locale to UTF-8
		// std::locale::global(std::locale("en_US.UTF-8"));

		// FIXME: Don't set fps and vertical sync at the same time
		m_window.setMouseCursorVisible(false);
		m_window.setFramerateLimit(config::FramesPerSecond);
		m_window.setVerticalSyncEnabled(config::VerticalSyncEnabled);

		// Create a default canvas
		using namespace utils;
		// ptr<Canvas> grid = createObject(new Canvas(config::CanvasSize, config::CanvasCellSize));
		ptr<Canvas> grid = createObject(make<Canvas>(
			config::CanvasSize, 
			config::CanvasCellSize)
		);

		// createObjects();
		// createObjects(
		// 	Image(""),
		// 	Canvas(nullptr, sf::Vector2u(800, 600))
		// );
		// createObject

		// /* Set app m_window size to grid size */
		config::WindowSize = sf::Vector2u(
			grid->getSize().x, 
			grid->getSize().y
		);

	}

	App::~App()
	{
		log::info("Shutting down application.");
		// Free all allocated objects
		// foreach(m_objects, [](ObjectPtr& p){
		//     if(p){
		//         delete p;
		//         p = nullptr;
		//     }
		// });

		ImGui::SFML::Shutdown();
	}

	App::ObjectPtr App::getObjectByIndex(sf::Uint64 index)
	{ return m_objects.at(index); }

	App::ObjectPtr App::getObjectByName(const std::string& name)
	{
		// FIXME: Will throw exception if fails
		return *findif(m_objects, [&name](ObjectPtr object){
			return object->getName() == name;
		});
	}

	template <ObjectType...Args>
	App::ObjectVector App::createObjects(Args...args)
	{
		ObjectVector objects;
		constexpr size_t targs_count = sizeof...(args);
		static_assert(targs_count > 0, "Function 'createObjects(Args...args)' requires at least one template argument");
		objects.emplace_back(createObject(std::forward<Args>(args)...));
		return objects;
	}

	App::ObjectPtr App::createObject(App::ObjectPtr object)
	{ 
		if(!object)
			return nullptr;
		object->setParent(this);
		m_objects.emplace_back(std::move(object)); 
		
		return createObject(object);
	}
	
	template <typename T>
	ptr<T> App::createObject(ptr<T> object)
	{
		if(!object)
			return nullptr;
		object->setParent(this);
		m_objects.emplace_back(object);
		std::sort(m_objects, [](const auto& a, const auto& b){ 
			return a->getDrawLayer() < b->getDrawLayer(); 
		});
		return object;
	}

	void App::removeObject(sf::Uint64 index)
	{  /* erase_and_remove(m_objects, index); */ }

	int App::runLoop()
	{
		sf::Time dt = m_clock.restart();
		while(m_isRunning && m_window.isOpen()){
			handleEvents(m_window, m_event);
			update(m_window, dt.asSeconds());
			render(m_window);
			// lastUpdate += dt;
			// m_threadPool.enqueue(
			//     [this](sf::Event& event){ 
			//         this->handleEvents(this->m_window, event);
			//     }, m_event
			// );
			// auto update = m_threadPool.enqueue(
			//     [this](float delta){
			//         this->update(delta);
			//     }, m_time.asSeconds()
			// );
			// m_threadPool.enqueue(
			//     [this]{
			//         this->render(this->m_window);
			//     }
			// );
			// while(lastUpdate > config::TimePerFrame)
			// {
			//     lastUpdate -= config::TimePerFrame;
				
			// }
		}
		return 0;
	}

	void App::handleEvents(sf::RenderWindow& window, sf::Event& event)
	{
		// Only call handleEvents() if events are queued
		while(window.pollEvent(event))
		{
			// processMessages();

			/* Handle ImGui events */
			ImGui::SFML::ProcessEvent(event);
			switch(event.type)
			{
				case sf::Event::Closed:
					m_isRunning = false;
					break;
				case sf::Event::MouseMoved:
				{
					config::MousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

					if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
					{
						// Get delta 
						const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
						// const sf::Vector2f deltaPos = config::LastMousePosition - newPos;
						sf::Vector2f deltaPos = (m_dragPosition - config::MousePosition)*m_cameraSensitivity;

						// Move our view accordingly and update the m_window
						sf::View view = window.getView();
						view.setCenter(view.getCenter() + deltaPos);
						window.setView(view);

						// Save the new position as the old one
						// We're recalculating this, since we've changed the view
						config::LastMousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
					}
					break;
				}
				case sf::Event::MouseButtonPressed:
					m_dragPosition = config::MousePosition;
					break;
				case sf::Event::MouseButtonReleased:
					break;
				case sf::Event::Resized:
					// sf::VideoMode vm = sf::VideoMode::getDesktopMode();
					// sf::Vector2u v = sf::Vector2u(vm.width, vm.height);
					// sf::Vector2u w = config::WindowSize;
					// sf::Vector2f scale = sf::Vector2f(v.x/w.x, v.y/w.y);
					break;
				
				case sf::Event::LostFocus:
					// Stop processing while m_window not focused
					break;
				case sf::Event::GainedFocus:
					// Continue processing when focused
					break;
				case sf::Event::MouseWheelScrolled:{
					if (event.mouseWheelScroll.delta > 0){
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_zoom > min(config::CameraZoom)){
							sf::Vector2i p = window.mapCoordsToPixel(config::MousePosition);
							zoomViewAt(p, window, 1.00 - m_zoomSensitivity);
							m_zoom -= m_zoomSensitivity;
						}
					}
					else if (event.mouseWheelScroll.delta < 0){
						if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_zoom < max(config::CameraZoom)){
							sf::Vector2i p = window.mapCoordsToPixel(config::MousePosition);
							zoomViewAt(p, window, 1.00 + m_zoomSensitivity);
							m_zoom += m_zoomSensitivity;
						}
					}
					break;
				}
			}
				
			foreach(m_objects, [&window, &event](ObjectPtr object){
				object->handleEvents(window, event);
			});
		}
	}

	void App::update(sf::RenderWindow& window, float deltaTime)
	{
		/* Update ImGui::SFML */
		ImGui::SFML::Update(window, m_clock.restart());
		if(ImGui::Begin("Brush", nullptr)){
			if(ImGui::BeginMainMenuBar()){
				m_enableDraw = !(ImGui::IsItemHovered() || ImGui::IsWindowHovered());
				if(ImGui::BeginMenu("File")){
					if(ImGui::MenuItem("New", "Ctrl+N")){
						// Create a new canvas object here
					}
					if(ImGui::MenuItem("Open...", "Ctrl+O")){
						m_imFileDialog.Open();
					}
					if(ImGui::MenuItem("Save", "Ctrl+S")){
						m_imFileDialog.Open();
					}
					if(ImGui::MenuItem("Resize Canvas")){

					}
					if(ImGui::MenuItem("Load Image...")){
						m_imFileDialog.Open();
					}
					
					if(ImGui::MenuItem("Preferences...")){
						if(ImGui::BeginChild("Preferences")){
							// int grid[2] = { (int)m_gridSize.x, (int)m_gridSize.y };
							// int cell[2] = { (int)m_cellSize.x, (int)m_cellSize.y };
							ImGui::Checkbox("Grid lines", &config::CanvasLines);
							ImGui::InputInt2("Canvas Size", (int*)&config::CanvasSize);
							ImGui::InputInt2("Cell Size", (int*)&config::CanvasCellSize);
							if(ImGui::Button("Update")){
							}
							ImGui::EndChild();
							
						}
					}
					if(ImGui::MenuItem("Close", "Ctrl+W")){
						// sendMessage(MessageFlags_CloseWindow);
					}
					
					ImGui::EndMenu();
				}
				
				if(ImGui::BeginMenu("About")){
					if(ImGui::MenuItem("License")){

					}
					if(ImGui::MenuItem("More Info...")){

					}
					ImGui::EndMenu();
				}
				m_enableDraw = !ImGui::IsWindowHovered();
				ImGui::EndMainMenuBar();
			}
			sf::Uint32 index = 0;
			foreach(ui::palette, [&index](sf::Color& color){
				std::string uid("color##" + std::to_string(index));
				if(ImGui::ColorButton(uid.data(), color))
					ui::brush.setPrimaryColor(color);
				ImGui::SameLine(0.0f, -0.1f);
				index += 1;
			});
			ImGui::ColorEdit4("Brush Color", (float*)&ui::imColorEdit, ImGuiColorEditFlags_PickerHueBar);
			m_enableDraw = !ImGui::IsWindowHovered();

			m_imFileDialog.Display();
			if(m_imFileDialog.HasSelected()){
				log::info("filename: {}", m_imFileDialog.GetSelected().string());
				m_imFileDialog.ClearSelected();
			}
			ImGui::End();
		}
		foreach(m_objects, [&window, &deltaTime](ObjectPtr object){
			object->update(window, deltaTime);
		});
	}

	void App::render(sf::RenderWindow& window)
	{
		m_window.clear(config::BackgroundColor);
		
		foreach(m_objects, [&window](ObjectPtr object){
			object->render(window);
		});
		ImGui::SFML::Render(window);
		window.display();
	}

	void App::parseArgs(int argc, char **argv)
	{
		cxxopts::Options opts(argv[0], "Simple pixel editor");
		opts.add_options()
			("v,verbose", "Enable verbose output")
			("octree", "Set number of octree partitions", cxxopts::value<int>())
			("t,test", "Run app tests")
			("set-framerate", "Set the app frame rate")
		;
		opts.allow_unrecognised_options();

		cxxopts::ParseResult result = opts.parse(argc, argv);
		// foreach(result.unmatched(), [](std::string& r){
		//     log::info("unrecognized option ", r);
		// });

	}
}
