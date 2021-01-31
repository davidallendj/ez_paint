
#include "Grid.hpp"
#include "AppConfig.hpp"
#include "Message.hpp"
#include "Quadtree.hpp"
#include "SpriteBatch.hpp"
#include "Utils.hpp"
#include <SFML/Config.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

namespace sp_hash
{
	#define Grid_INIT 						\
	: m_gridSize(gridSize) 					\
	, m_cellSize(cellSize) 					\
	, m_gridShadow() 						\
	, m_shadowShader(new sf::Shader()) 		\
	, m_texture(nullptr) 					\
	, m_position(sf::Vector2u(0, 0)) 		\
	, m_brush(Brush(20)) 					\
	, m_enableDraw(true) 					\
	, m_quadtree(m_vertexArray.getBounds()) \
	, m_colorEdit(0.0, 0.0, 0.0, 0.0) 		\
	, m_vertexArray()						\
	, m_paletteColors{ 						\
		sf::Color::Black, 					\
		sf::Color(102, 102, 102, 255), 		\
		sf::Color::White, 					\
		sf::Color(128, 0, 0, 255), 			\
		sf::Color::Red, 					\
		sf::Color(255, 102, 0, 255), 		\
		sf::Color::Yellow, 					\
		sf::Color::Green, 					\
		sf::Color::Cyan, 					\
		sf::Color(0, 155, 255, 255), 		\
		sf::Color::Blue, 					\
		sf::Color(128, 0, 255, 255), 		\
		sf::Color(191, 0, 255, 255), 		\
		sf::Color::Magenta, 				\
		sf::Color(255, 0, 191, 255) 		\
	}										

	Grid::Grid(Object *parent, const sf::Vector2u& gridSize, const sf::Vector2u& cellSize)
	Grid_INIT
	, Object::Object(parent)
	{ 
		m_quadtree.setBounds(m_vertexArray.getBounds()); 
		m_quadtree.partition(AppConfig::QuadtreePartitions);
	}

    Grid::Grid(const sf::Vector2u& gridSize, const sf::Vector2u& cellSize)
	Grid_INIT
	, Object::Object(nullptr)
	{
		m_shadowShader->loadFromFile("shaders/DropShadow.frag", sf::Shader::Fragment);
		m_shadowShader->setUniform("mask", sf::Glsl::Vec4(0, 0, 0, 50));
		m_shadowShader->setUniform("direction", sf::Vector2f(0.f, 1.0f));

		m_vertexArray.setPrimitiveType(sf::Quads);
		m_vertexArray.resize(gridSize.x * gridSize.y * 4);

		batch();
		sf::FloatRect bounds = m_vertexArray.getBounds();
		m_quadtree.setBounds(bounds);
		m_quadtree.partition(AppConfig::QuadtreePartitions);
		
		m_gridShadow.setPosition(sf::Vector2f(bounds.left-4, bounds.top-4));
		m_gridShadow.setSize(sf::Vector2f(bounds.width+8, bounds.height+8));
		m_gridShadow.setFillColor(sf::Color(150, 150, 150, 100));
		m_gridShadow.setOutlineColor(sf::Color(100, 100, 100, 50));
		m_gridShadow.setOutlineThickness(0.2);
	}

	Grid::~Grid()
	{ delete m_shadowShader; }

	void Grid::handleEvents(sf::RenderWindow& window, sf::Event& event)
	{
		switch(event.type)
		{
			case sf::Event::MouseButtonPressed:
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_enableDraw)
					hashBrush();	
				break;
			case sf::Event::MouseButtonReleased:
				break;
			case sf::Event::MouseMoved:
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_enableDraw)
					hashBrush();
				break;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			// sf::FloatRect bounds = m_spriteBatch->getVertexArray().getBounds();
			// if (bounds.contains(AppConfig::mousePosition))
			// {}
		}
		m_brush.handleEvents(window, event);
		// m_enableDraw = true;
	}

	void Grid::update(float deltaTime)
	{
		if(ImGui::Begin("Brush", nullptr))
		{
			if(ImGui::BeginMainMenuBar())
			{
				m_enableDraw = !(ImGui::IsAnyWindowHovered() || ImGui::IsItemClicked());
				if(ImGui::BeginMenu("File"))
				{
					if(ImGui::MenuItem("Open...", "Ctrl+O")){
						// ImGui::FileDialog(){}
					}
					if(ImGui::MenuItem("Save", "Ctrl+S")){
						// ImGui::FileDialog(){}
					}
					if(ImGui::MenuItem("Options"))
					{
						if(ImGui::Begin("Set Options##", nullptr))
						{
							int grid[2] = { (int)m_gridSize.x, (int)m_gridSize.y };
							int cell[2] = { (int)m_cellSize.x, (int)m_cellSize.y };
							ImGui::InputInt2("Grid Size", (int*)&m_gridSize);
							ImGui::InputInt2("Cell Size", (int*)&m_cellSize);
							if(ImGui::Button("Update"))
							{

							}
							ImGui::End();
							
						}
					}
					if(ImGui::MenuItem("Close", "Ctrl+W")){
						// sendMessage(MessageFlags_CloseWindow);
					}
					
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("About"))
				{
					if(ImGui::MenuItem("License"))
					{

					}
					if(ImGui::MenuItem("More Info..."))
					{

					}
					ImGui::EndMenu();
				}
				m_enableDraw = !ImGui::IsWindowHovered();
				ImGui::EndMainMenuBar();
			}
			sf::Uint32 index = 0;
			foreach(m_paletteColors, [this, &index](sf::Color& color){
				sf::String uid("color##" + std::to_string(index));
				if(ImGui::ColorButton(uid.toAnsiString().c_str(), color))
					m_brush.setPrimaryColor(color);
				ImGui::SameLine(0.0f, -0.1f);
				index += 1;
			});
			float c[] = {0.0, 0.0, 0.0, 0.0};
			ImGui::ColorEdit4("Brush Color", (float*)&m_colorEdit, ImGuiColorEditFlags_PickerHueBar);
			m_enableDraw = !ImGui::IsWindowHovered();
			ImGui::End();

			// m_enableDraw = false;
		}
		
		m_brush.update(deltaTime);
		// setHashColors();
	}

	void Grid::render(sf::RenderWindow& window)
	{
		// window.draw(*m_spriteBatch);
		window.draw(m_gridShadow);
		window.draw(m_vertexArray);
		m_brush.render(window);
	}

	void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.shader = m_shadowShader;
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertexArray, states);
	}

	void Grid::hashBrush()
	{
		// const QuadtreeNode *node = m_quadtree.search(m_brush.getBounds());
		Quadtree::QuadtreeNodeVector nodes = m_quadtree.search(m_brush.getBounds());
		foreach(nodes, [this](const QuadtreeNode *node){
			const sf::FloatRect& bounds = node->getBounds();
			sf::Uint32 i = static_cast<sf::Uint32>(bounds.left / m_cellSize.x);
			sf::Uint32 j = static_cast<sf::Uint32>(bounds.top / m_cellSize.y);
			sf::Vertex *quad = getQuad(sf::Vector2u(i, j));
			sf::Color color = m_brush.getPrimaryColor();
			setQuadColor(quad, color);
		});
	}

	void Grid::batch(sf::Color color, sf::PrimitiveType type)
	{
		for(sf::Uint32 j = 0; j < m_gridSize.y; ++j)
		{
			for(sf::Uint32 i = 0; i < m_gridSize.x; ++i)
			{
				sf::Vertex *vertex = &m_vertexArray[(i + j * m_gridSize.x) * 4];
                if (type == sf::LinesStrip)
                {
                    vertex[0].position = sf::Vector2f(i * m_cellSize.x, j * m_cellSize.y);
                    vertex[1].position = sf::Vector2f((i + 1) * m_cellSize.x, j * m_cellSize.y);
                    vertex[2].position = sf::Vector2f((i + 1) * m_cellSize.x, (j + 1) * m_cellSize.y);
                    vertex[3].position = sf::Vector2f(i * m_cellSize.x, (j + 1) * m_cellSize.y);
                }
                else
                {
                    // Define its 4 corners
                    vertex[0].position = sf::Vector2f(i * m_cellSize.x, j * m_cellSize.y);
                    vertex[1].position = sf::Vector2f((i + 1) * m_cellSize.x, j * m_cellSize.y);
                    vertex[2].position = sf::Vector2f((i + 1) * m_cellSize.x, (j + 1) * m_cellSize.y);
                    vertex[3].position = sf::Vector2f(i * m_cellSize.x, (j + 1) * m_cellSize.y);
                }
				setQuadColor(vertex, color);
			}
		}
	}

	void Grid::setQuadAlpha(sf::Uint8 alpha)
	{
		if (m_vertexArray.getVertexCount() > 0)
			for (sf::Uint8 i = 0; i < m_paletteColors.size(); ++i)
				setQuadAlpha(getQuad(i), alpha);
	}

	void Grid::setQuadAlpha(sf::Vertex *quad, sf::Uint8 alpha)
	{
		quad[0].color.a = alpha;
		quad[1].color.a = alpha;
		quad[2].color.a = alpha;
		quad[3].color.a = alpha;
	}

	void Grid::setQuadColor(sf::Vertex *quad, const sf::Color& color)
	{
		quad[0].color = color;
		quad[1].color = color;
		quad[2].color = color;
		quad[3].color = color;
	}

	void Grid::setQuadPosition(sf::Vertex *quad, sf::Vector2f position)
	{
		quad[0].position = sf::Vector2f(position.x * m_cellSize.x, position.y * m_cellSize.y);
		quad[1].position = sf::Vector2f(position.x * m_cellSize.x, position.y * m_cellSize.y);
		quad[2].position = sf::Vector2f(position.x * m_cellSize.x, position.y * m_cellSize.y);
		quad[3].position = sf::Vector2f(position.x * m_cellSize.x, position.y * m_cellSize.y);
	}

} // namespace sf