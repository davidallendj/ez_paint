
#pragma once

#include "message.hpp"
#include "log.hpp"
#include "counter.hpp"
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <type_traits>


namespace ez_paint
{
	class Object : counter<Object>
	{
	public:
		explicit Object(Object *parent = nullptr, const std::string& name = "object");
		explicit Object(const std::string& name);
		virtual ~Object(){}

		void setActive(bool isActive);
		void setName(const std::string& name);
		void setParent(Object *parent);


		bool isActive() const;
		const std::string& getName() const;
		sf::Uint32 getDrawLayer() const;
		sf::Uint32 getId() const;
		Object* getParent() const;

		template <typename T = MessageFlags_>
		void sendMessage(Message<T>&& message, MessageReceiver<T>& who);
		template <typename T = MessageFlags_>
		void sendMessage(T&& message, MessageReceiver<T>& who);

		virtual void init() {}
		virtual void clean() {}

		virtual void handleEvents(sf::RenderWindow& window, sf::Event& event) = 0;
		virtual void update(sf::RenderWindow& window, float deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;

	private:
		Object *m_parent;
		sf::Uint32 m_id;
		sf::Uint32 m_drawLayer;
		std::string m_name;
		bool m_isActive;
	};

	#define Object_INIT 					\
	, m_id(0)								\
	, m_drawLayer(0)						\
	, m_name(name)							\
	, m_isActive(true)						\
	{										\
		m_id = counter<ez_paint::Object>::m_created++;		\
		log::info("Created '{} (id: {})'", m_name, m_id); 	\
	}

	inline Object::Object(Object *parent, const std::string& name)
	: m_parent(parent)
	Object_INIT

	inline Object::Object(const std::string& name)
	: m_parent(nullptr)
	Object_INIT
	

	inline void Object::setActive(bool isActive)
	{ m_isActive = isActive; }

	inline void Object::setName(const std::string& name)
	{ m_name = name; }

	inline void Object::setParent(Object *parent)
	{ m_parent = parent; }

	inline bool Object::isActive() const
	{ return m_isActive; }

	inline const std::string& Object::getName() const
	{ return m_name; }

	inline sf::Uint32 Object::getDrawLayer() const
	{ return m_drawLayer; }

	inline sf::Uint32 Object::getId() const
	{ return m_id; }

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

	std::ostream& operator<<(std::ostream& os, const Object& o);
	template <typename T> concept ObjectType = std::is_base_of_v<Object, T>;
	template <typename T, template <typename> typename P> 
	concept ObjectPtrType = std::is_pointer_v<P<T>> && std::is_base_of_v<Object, T>;
}
