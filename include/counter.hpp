#pragma once

#include <cstddef>

namespace ez_paint
{
	template <typename T>
	class counter
	{
	public:
		counter(){ m_created++; m_alive++; }

	protected:
		virtual ~counter(){ --m_alive; }
		static size_t m_created;
		static size_t m_alive;
	};

	template <typename T> size_t counter<T>::m_created(0);
	template <typename T> size_t counter<T>::m_alive(0);
}