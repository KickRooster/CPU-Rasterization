#include "MathBase.h"

namespace core
{
    bool Equal(float Lhs, float Rhs)
    {
        return abs(Lhs - Rhs) < FLOAT_ERROR;
    }

    float Degree2Radian(float Degree)
    {
        return Degree * DEGREE_TO_RADIAN;
    }

    float Radian2Degree(float Radian)
    {
        return Radian * RADIAN_TO_DEGREE;
    }
}
