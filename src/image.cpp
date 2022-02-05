

#include "image.hpp"

namespace fu
{
	Image::Image(const std::string& path)
	: Object::Object("image")
	{
		setName("Image");
	}

	Image::Image(const sf::Texture& texture)
	: Object::Object("image")
	, m_sprite(texture)
	{
		m_sprite.setPosition(sf::Vector2f(0, 0));
	}

	void Image::handleEvents(sf::RenderWindow &window, sf::Event &event)
	{

	}

	void Image::update(sf::RenderWindow& window, float dt)
	{

	}

	void Image::render(sf::RenderWindow &window)
	{
		window.draw(m_sprite);
	}
}