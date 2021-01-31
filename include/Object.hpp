
#ifndef SP_HASH_OBJECT_HPP
#define SP_HASH_OBJECT_HPP

#include "Utils.hpp"
#include "Message.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


namespace sp_hash
{
	class Object
	{
	public:
		Object(Object *parent = nullptr);
		virtual ~Object(){}

		bool isActive() const;
		void setActive(bool isActive);
		const sf::String& getName() const;
		void setName(const sf::String& name);
		void setParent(Object *parent);
		Object* getParent() const;

		template <typename T = MessageFlags_>
		void sendMessage(Message<T>&& message, MessageReceiver<T>& who);
		template <typename T = MessageFlags_>
		void sendMessage(T&& message, MessageReceiver<T>& who);

		virtual void handleEvents(sf::RenderWindow& window, sf::Event& event) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;

	private:
		Object *m_parent;
		bool m_isActive;
		sf::String m_name;
		sf::Uint32 m_drawLayer;
	};

	inline Object::Object(Object *parent)
	: m_parent(parent)
	{}
	
	inline bool Object::isActive() const
	{ return m_isActive; }

	inline void Object::setActive(bool isActive)
	{ m_isActive = isActive; }

	inline const sf::String& Object::getName() const
	{ return m_name; }

	inline void Object::setName(const sf::String& name)
	{ m_name = name; }

	inline void Object::setParent(Object *parent)
	{ m_parent = parent; }

	inline Object* Object::getParent() const
	{ return m_parent; }
	

	template <typename T>
	void sendMessage(Message<T>&& message, MessageReceiver<T>& who)
	{

	}

	template <typename T>
	void sendMessage(T&& message, MessageReceiver<T>& who)
	{

	}



	std::ostream& operator<<(std::ostream& os, Object& o);
}

#endif // SP_HASH_OBJECT_HPP