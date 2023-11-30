#include "Joystick.h"

namespace core
{
    FJoystick::FJoystick():
    WDown(false),
    SDown(false),
    ADown(false),
    DDown(false),
    QDown(false),
    EDown(false),
    LeftShiftDown(false),
    LeftButtonDown(false),
    MiddleButtonDown(false),
    RightButtonDown(false),
    XPos(0),
    YPos(0),
    ZPos(1.0f)
    {
    }
}
