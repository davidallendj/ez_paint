#pragma once

#include "config.hpp"
#include "message.hpp"
#include "object.hpp"
#include "utils.hpp"
#include "types.hpp"
#include "thread_pool.hpp"
#include "canvas_layer.hpp"
#include "brush.hpp"

#include <bitset>
#include <vector>
#include <memory>
#include <variant>
#include <any>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"


namespace ez_paint
{
	namespace ui
	{
		using namespace types;
		extern float imColorEdit;
		extern Brush brush;
		extern Palette palette;
	};

	class App : public MessageReceiver<>, Object
	{
	public:
		typedef utils::ptr<Object>			ObjectPtr;
		typedef utils::vector<ObjectPtr>	ObjectVector;

		App(int argc = 0, char **argv = nullptr);
		virtual ~App();

		template <typename T>
		utils::ptr<T> createObject(utils::ptr<T> object);
		template <ObjectType...Args>
		ObjectVector createObjects(Args...args);
		ObjectPtr createObject(ObjectPtr object);
		ObjectPtr getObjectByIndex(sf::Uint64 index);
		ObjectPtr getObjectByName(const std::string& name);
		void removeObject(sf::Uint64 index);
		int runLoop();

		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(sf::RenderWindow& window, float deltaTime);
		void render(sf::RenderWindow& window);

	private:
		bool m_isRunning;
		ObjectVector m_objects;
		ThreadPool m_threadPool;
		sf::RenderWindow m_window;
		sf::Event m_event;
		sf::Clock m_clock;
		sf::Time m_time;

		bool m_enableDraw;
		ImGui::FileBrowser m_imFileDialog;

		

		// Dragging vars
		std::bitset<8> m_keyModifiers;
		sf::Vector2f m_dragPosition;
		float m_zoom;
		float m_zoomSensitivity;
		float m_cameraSensitivity;

		void parseArgs(int argc, char **argv);
	};
}
