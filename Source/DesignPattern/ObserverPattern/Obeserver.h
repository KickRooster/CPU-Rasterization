#pragma once
#include <Core.h>

namespace core
{
	class Obeserver
	{
	public:
		Obeserver() {}
		virtual void UpdateAhead() = 0;
		virtual void UpdateBehind() = 0;
		virtual ~Obeserver() {}
	};
}
