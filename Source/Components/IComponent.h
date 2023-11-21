#pragma once
#include <Core.h>

namespace core
{
    class IComponent
    {
    public:
        virtual void OnRegister() = 0;
        virtual void OnUnregister() = 0;
        virtual ~IComponent(){}
    };
}
