#pragma once
#include <Core.h>
#include "World.h"
#include "../Rasterizer/IrradianceBuffer.h"
#include "../Others/Joystick.h"

namespace core
{
    class UGame
    {
        std::unique_ptr<UWorld> CurrentWorld;
        std::unique_ptr<FIrradianceBuffer> IrradianceBuffer;

    public:
        void Initialize(int32 ViewportWidth, int32 ViewportHeight);
        void Tick(const FJoystick& Joystick, float DeltaTime);
        void Render();
        uint8 * GetLDRData() const;
        void ShutDown();
    };
}
