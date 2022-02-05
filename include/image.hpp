
#ifndef FU_IMAGE_HPP
#define FU_IMAGE_HPP

#include "object.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace fu
{
	class Image : public Object
	{
	public:
		// Allow implicit conversion of param -> Image
		Image(const std::string& path);
		Image(const sf::Texture& texture);
		void handleEvents(sf::RenderWindow& window, sf::Event& event);
		void update(sf::RenderWindow& window, float dt);
		void render(sf::RenderWindow& window);

	private:
		sf::Sprite m_sprite;
	};

	
}

#endif // FU_IMAGE_HPP