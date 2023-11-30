#pragma once
#include <core.h>

namespace core
{
    struct FJoystick
    {
        bool WDown;
        bool SDown;
        bool ADown;
        bool DDown;
        bool QDown;
        bool EDown;
        bool LeftShiftDown;

        bool LeftButtonDown;
        bool MiddleButtonDown;
        bool RightButtonDown;
        int32 XPos;
        int32 YPos;
        int32 ZPos;

        FJoystick();
    };
}
