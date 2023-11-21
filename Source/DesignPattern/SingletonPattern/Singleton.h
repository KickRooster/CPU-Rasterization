#pragma once
#include <Core.h>

namespace core
{
	template<typename T>
	class Singleton
	{
	public:
		static T * Instance()
		{
			static T instance;

			return &instance;
		}

	protected:
		Singleton() {}
		Singleton(Singleton const&) {}
		Singleton& operator=(Singleton const&) {}
		~Singleton() {}
	};
}
