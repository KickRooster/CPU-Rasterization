#include "MathBase.h"

namespace core
{
    bool Equal(float Lhs, float Rhs)
    {
        return abs(Lhs - Rhs) < FLOAT_ERROR;
    }
}
